#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cstdio>
#include "Pipeline.h"
#include "GLProgram.h"
using namespace std;

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

GLuint VBO;

GLuint gWorldLocation;


void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.1f;
	Scale += 0.01f;

	Pipeline p;
	p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	p.WorldPos(sinf(Scale), 0.0f, 0.0f);
	//p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

	glm::vec3 CameraPos{ 1.0f, 1.0f, -3.0f };
	glm::vec3 CameraTarget{ 0.45f, 0.0f, 1.0f };
	glm::vec3 CameraUp{ 0.0f, 1.0f, 0.0f };
	p.SetCamera(CameraPos, CameraTarget, CameraUp);

	p.SetPerspectiveProj(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &(p.GetTrans()[0][0]));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

const GLchar pVS[] = "#version 330\n layout (location = 0) in vec3 Position; uniform mat4 gWorld; void main() {gl_Position = gWorld * vec4(Position, 1.0);}";
const GLchar pFS[] = "#version 330\n out vec4 FragColor; void main() {FragColor = vec4(0.5, 0.5, 0.5, 1.0);}";

void prepareVertices() {
	glm::vec3 vectors[3] = {
		{ -0.9f, -0.9f, 0.1f},
		{ 0.9f, -0.9f, 0.1f},
		{ 0.1f, 0.9f, 0.9f},
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Amir");
	glutDisplayFunc(render);
	glutIdleFunc(render);


	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		throw "error";
	}

	prepareVertices();

	GLShaderProgram shader;
	shader.addShader(GL_VERTEX_SHADER, pVS);
	shader.addShader(GL_FRAGMENT_SHADER, pFS);
	shader.linkProgram();
	shader.use();
	gWorldLocation = shader.getUniformLocation("gWorld");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	glutMainLoop();
}
