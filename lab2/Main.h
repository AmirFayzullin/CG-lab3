#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "ICallbacks.h"
#include "Pipeline.h"
#include "Texture.h"
#include "LightingProgram.h"

struct Vertex {
    glm::vec3 fst;
    glm::vec2 snd;

    Vertex(glm::vec3 inp1, glm::vec2 inp2) {
        fst = inp1;
        snd = inp2;
    }
};

class Main : public ICallbacks {
    int WINDOW_WIDTH = 1024;
    int WINDOW_HEIGHT = 768;

    GLuint gWorldLocation;
    GLuint VBO;
    GLuint IBO;
    Texture* pTexture = nullptr;
    LightingProgram* pLighting = nullptr;
    const char* texturePath = "C:\\Users\\Amir\\Desktop\\For UGATU\\Computer graphics\\lab3\\lab2\\test.jpg";

    DirectionLight dirLight = { {0.9f, 0.9f, 0.9f}, 0.5f };


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
        v += 0.001f;

        Pipeline p;
        p.Scale(0.6f, 0.6f, 0.6f);
        p.WorldPos(0.1f, 0.1f, 0.5f);
        p.Rotate(sinf(v) * 90, sinf(v) * 90, 0.1f);
        pLighting->setGWP(&(p.GetTrans()[0][0]));
        pLighting->setDirectionalLight(dirLight);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        pTexture->Bind(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glutSwapBuffers();
    }

    void IdleCB() override {
        RenderSceneCB();
    }
};