#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "ICallbacks.h"
#include "Pipeline.h"
#include "Texture.h"
#include "LightingProgram.h"

class Main : public ICallbacks {
	int WINDOW_WIDTH = 1024;
	int WINDOW_HEIGHT = 768;
	
	GLuint gWorldLocation;
	GLuint VBO;
	Texture* pTexture = nullptr;
	LightingProgram* pLighting = nullptr;
	const char* texturePath = "C:/Users/Amir/Desktop/For UGATU/Computer graphics/lab3/lab2/bauble.png";

	DirectionLight dirLight = { {1.0f, 0.1f, 0.1f}, 0.5f };


	void CreateVertices() {
		glm::vec3 vectors[3] = {
			{ -0.9f, -0.9f, 0.1f},
			{ 0.9f, -0.9f, 0.1f},
			{ 0.1f, 0.9f, 0.9f},
		};

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);
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

		static float Scale = 0.1f;
		Scale += 0.01f;

		Pipeline p;
		p.Scale(0.3f, 0.3f, 0.3f);
		p.WorldPos(-0.5f, 1.0f, 0.0f);
		p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(1.0f) * 90.0f);

		glm::vec3 CameraPos{ 1.0f, 1.0f, -3.0f };
		glm::vec3 CameraTarget{ 0.45f, 0.0f, 1.0f };
		glm::vec3 CameraUp{ 0.0f, 1.0f, 0.0f };
		p.SetCamera(CameraPos, CameraTarget, CameraUp);

		p.SetPerspectiveProj(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
		//glUniformMatrix4fv( gWorldLocation, 1, GL_TRUE, &(p.GetTrans()[0][0]));
		pLighting->setGWP(&(p.GetTrans()[0][0]));
		pLighting->setDirectionalLight(dirLight);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		pTexture->Bind(GL_TEXTURE0);

		glDisableVertexAttribArray(0);

		glutSwapBuffers();
	}

	void IdleCB() override {
		RenderSceneCB();
	}
};
