#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "ICallbacks.h"
#include "Pipeline.h"
#include "Texture.h"
#include "LightingProgram.h"
#include "Math_3d.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct Vertex {
    glm::vec3 pos;
    glm::vec2 tex;
    glm::vec3 normal;

    Vertex() {}

    Vertex(glm::vec3 _pos, glm::vec2 _tex) {
        pos = _pos;
        tex = _tex;
        normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

class Main : public ICallbacks {
    int WINDOW_WIDTH = 1024;
    int WINDOW_HEIGHT = 768;

    GLuint VBO;
    GLuint IBO;
    Texture* pTexture = nullptr;
    LightingProgram* pLighting = nullptr;
    const char* texturePath = "C:\\Users\\Amir\\Desktop\\For UGATU\\Computer graphics\\lab3\\lab2\\texture.jpg";

    DirectionalLight dirLight = { {0.9f, 0.9f, 0.9f}, 0.0f, {1.0f, 0.0f, 0.0f}, 0.75f};

    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
    {
        for (unsigned int i = 0; i < IndexCount; i += 3) {
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            glm::vec3 v1 = pVertices[Index1].pos - pVertices[Index0].pos;
            glm::vec3 v2 = pVertices[Index2].pos - pVertices[Index0].pos;
            glm::vec3 Normal = VectorsMath::Cross(v1, v2);
            VectorsMath::Normalize(Normal);

            pVertices[Index0].normal += Normal;
            pVertices[Index1].normal += Normal;
            pVertices[Index2].normal += Normal;
        }

        for (unsigned int i = 0; i < VertexCount; i++) {
            VectorsMath::Normalize(pVertices[i].normal);
        }
    }


    void CreateVertices() {
        unsigned int Indices[] = {
             0, 3, 1,
             1, 3, 2,
             2, 3, 0,
             1, 2, 0
        };

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

        Vertex input[4] = {
          Vertex(glm::vec3 {-1.0f, -1.0f, 0.5773f}, glm::vec2 {0.0f, 0.0f}),
          Vertex(glm::vec3 {0.0f, -1.0f, -1.1547}, glm::vec2 {0.5f, 0.0f}),
          Vertex(glm::vec3 {1.0f, -1.0f, 0.5773f}, glm::vec2 {1.0f, 0.0f}),
          Vertex(glm::vec3 {0.0f, 1.0f, 0.0f}, glm::vec2 {0.5f, 1.0f}),
        };

        unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(input);

        CalcNormals(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices), input, VertexCount);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);
    }

    void setPointLights() {
        static float scale = 0.1f;
        scale += 0.01f;

        PointLight pl[3];
        pl[0].DiffuseIntensity = 0.8f;
        pl[0].Color = glm::vec3(1.0f, 1.0f, 0.0f);
        pl[0].Position = glm::vec3(sinf(scale) * 10, 1.0f, cosf(scale) * 10);
        pl[0].Attenuation.Linear = 0.1f;

        pl[1].DiffuseIntensity = 0.8f;
        pl[1].Color = glm::vec3(0.0f, 1.0f, 0.0f);
        pl[1].Position = glm::vec3(sinf(scale + 2.1f) * 10, 1.0f, cosf(scale + 2.1f) * 10);
        pl[1].Attenuation.Linear = 0.1f;

        pl[2].DiffuseIntensity = 0.8f;
        pl[2].Color = glm::vec3(0.0f, 0.0f, 1.0f);
        pl[2].Position = glm::vec3(sinf(scale + 4.2f) * 10, 1.0f, cosf(scale + 4.2f) * 10);
        pl[2].Attenuation.Linear = 0.1f;

        pLighting->setPointLights(3, pl);
    }

public:
    ~Main() {
        delete pTexture;
        delete pLighting;
    }

    bool Init() {
        CreateVertices();

        pLighting = new LightingProgram();

        if (!pLighting->init()) return false;

        pLighting->use();

        pLighting->setTextureUnit(0);

        pTexture = new Texture(GL_TEXTURE_2D, texturePath);

        if (!pTexture->Load()) {
            return false;
        }

        return true;
    }

    void RenderSceneCB() override {
        glClear(GL_COLOR_BUFFER_BIT);

        setPointLights();

        static float v = 0.1f;
        v += 0.01f;

        Pipeline p;
        p.Scale(0.7f, 0.7f, 0.7f);
        p.WorldPos(0.0f, 0.0f, 1.0f);
        p.Rotate(0.0f, sinf(v) * 90.0f, 0.0f);

        const glm::mat4x4& WorldTransformation = p.getWorldTrans();
        pLighting->setGWP(WorldTransformation);
        pLighting->setWVP(WorldTransformation);

        pLighting->setDirLight(dirLight);
        glm::vec3 cameraPos = { 0.0f, 0.0f, 0.0f };
        pLighting->setEyeWorldPos(cameraPos);
        pLighting->setSpecularIntensity(0.5f);
        pLighting->setSpecularPower(32);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        pTexture->Bind(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glutSwapBuffers();
    }

    void IdleCB() override {
        RenderSceneCB();
    }
};