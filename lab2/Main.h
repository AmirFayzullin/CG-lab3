#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "ICallbacks.h"
#include "Pipeline.h"
#include "Texture.h"
#include "LightingProgram.h"

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
    const char* texturePath = "C:\\Users\\Amir\\Desktop\\For UGATU\\Computer graphics\\lab3\\lab2\\test.jpg";

    DirectionLight dirLight = { {0.9f, 0.9f, 0.9f}, 0.5f, {0.5f, 0.5f, 0.5f}, 1.0f};

    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
    {
        for (unsigned int i = 0; i < IndexCount; i += 3) {
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            glm::vec3 v1 = pVertices[Index1].pos - pVertices[Index0].pos;
            glm::vec3 v2 = pVertices[Index2].pos - pVertices[Index0].pos;
            glm::vec3 Normal = glm::cross(v1, v2);
            glm::normalize(Normal);

            pVertices[Index0].normal += Normal;
            pVertices[Index1].normal += Normal;
            pVertices[Index2].normal += Normal;
        }

        for (unsigned int i = 0; i < VertexCount; i++) {
            glm::normalize(pVertices[i].normal);
        }
    }


    void CreateVertices() {
        Vertex input[4] = {
          Vertex(glm::vec3 {-1.0f, -1.0f, 0.5773f}, glm::vec2 {0.0f, 0.0f}),
          Vertex(glm::vec3 {0.0f, -1.0f, -1.1547}, glm::vec2 {0.5f, 0.0f}),
          Vertex(glm::vec3 {1.0f, -1.0f, 0.5773f}, glm::vec2 {1.0f, 0.0f}),
          Vertex(glm::vec3 {0.0f, 1.0f, 0.0f}, glm::vec2 {0.5f, 1.0f}),
        };

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);

        unsigned int Indices[] = { 0, 3, 1,
                     1, 3, 2,
                     2, 3, 0,
                     1, 2, 0 };

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
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

        static float v = 0.1f;
        v += 0.01f;

        Pipeline p;
        p.Scale(0.4f, 0.4f, 0.4f);
        //p.WorldPos(0.3f, 0.1f, 1.0f);
        p.Rotate(0.0f, sinf(v) * 90.0f, sinf(v) * 90.0f);

        glm::vec3 CameraPos{ 0.5f, 0.7f, 2.0f };
        glm::vec3 CameraTarget{ 0.45f, 0.0f, 1.0f };
        glm::vec3 CameraUp{ 0.0f, 1.0f, 0.0f };
        p.SetCamera(CameraPos, CameraTarget, CameraUp);

        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);

        pLighting->setWVP(&(p.getTransformationMatrix()[0][0]));
        pLighting->setWorldMatrix(&(p.getTransformationMatrix()[0][0]));
        pLighting->setDirectionalLight(dirLight);

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