#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cstdio>
#include "Pipeline.h"
#include "LightingProgram.h"
#include "Texture.h"
#include "GLUTBackend.h"
#include "Main.h"
using namespace std;

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

//GLuint VBO;
//GLuint gSampler;
//Texture* pTexture = nullptr;
//GLuint gWorldLocation;


//void render() {
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	static float Scale = 0.1f;
//	Scale += 0.01f;
//
//	Pipeline p;
//	p.Scale(0.3f, 0.3f, 0.3f);
//	p.WorldPos(-0.5f, 1.0f, 0.0f);
//	p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(1.0f) * 90.0f);
//
//	glm::vec3 CameraPos{ 1.0f, 1.0f, -3.0f };
//	glm::vec3 CameraTarget{ 0.45f, 0.0f, 1.0f };
//	glm::vec3 CameraUp{ 0.0f, 1.0f, 0.0f };
//	p.SetCamera(CameraPos, CameraTarget, CameraUp);
//
//	p.SetPerspectiveProj(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
//	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &(p.GetTrans()[0][0]));
//
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	pTexture->Bind(GL_TEXTURE0);
//
//	glDisableVertexAttribArray(0);
//
//	glutSwapBuffers();
//}

//void prepareVertices() {
//	glm::vec3 vectors[3] = {
//		{ -0.9f, -0.9f, 0.1f},
//		{ 0.9f, -0.9f, 0.1f},
//		{ 0.1f, 0.9f, 0.9f},
//	};
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);
//}

//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//	glutInitWindowPosition(50, 50);
//	glutCreateWindow("Amir");
//	glutDisplayFunc(render);
//	glutIdleFunc(render);
//
//
//	GLenum res = glewInit();
//	if (res != GLEW_OK)
//	{
//		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
//		throw "error";
//	}
//
//	Magick::InitializeMagick(*argv);
//
//	GLShaderProgram shader;
//	shader.addShader(GL_VERTEX_SHADER, pVS);
//	shader.addShader(GL_FRAGMENT_SHADER, pFS);
//	shader.linkProgram();
//	shader.use();
//	gWorldLocation = shader.getUniformLocation("gWorld");
//	gSampler = shader.getUniformLocation("gSampler");
//
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
//
//	glUniform1i(gSampler, 0);
//
//	prepareVertices();
//
//	pTexture = new Texture(GL_TEXTURE_2D, "C:/Users/Amir/Desktop/For UGATU/Computer graphics/lab3/lab2/bauble.png");
//
//	if (!pTexture->Load()) {
//		return 1;
//	}
//
//	glutMainLoop();
//}


//const GLchar pVS[] = "\n\
//#version 330\n\
//layout (location = 0) in vec3 Position; \n\
//layout (location = 1) in vec2 TexCoord; \n\
//uniform mat4 gWorld; \n\
//out vec2 TexCoord0;  \n\
//void main() { \n\
//	gl_Position = gWorld * vec4(Position, 1.0); \n\
//	TexCoord0 = TexCoord;\n\
//}";
//const GLchar pFS[] = "\n\
//#version 330\n\
//in vec2 TexCoord0; \n\
//out vec4 FragColor; \n\
//uniform sampler2D gSampler;\n\
//void main() {\n\
//	FragColor = texture2D(gSampler, TexCoord0.xy);\n\
//}";


int main(int argc, char** argv) {
	GLUTBackend::Init(argc, argv);
	GLUTBackend::CreateWin(WINDOW_WIDTH, WINDOW_HEIGHT, 50, 50, "Amir");

	Magick::InitializeMagick(*argv);

	Main *app = new Main();
	
	if (!app->Init()) return 1;

	GLUTBackend::Run(app);

	delete app;
}