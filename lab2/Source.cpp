#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <cstdio>
using namespace std;

#define M_PI 3.14159265358979323846
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

GLuint VBO;

GLuint gWorldLocation;

class Pipeline {
	glm::vec3 
		vScale{ 0.0f, 0.0f, 0.0f },
		vRotate{ 0.0f, 0.0f, 0.0f },
		vTranslation{ 0.0f, 0.0f, 0.0f };
	glm::mat4 mTransformation;

	glm::mat4 InitScaleTransform() {
		return glm::mat4{
			{vScale[0], 0.0f, 0.0f, 0.0f},
			{0.0f, vScale[1], 0.0f, 0.0f},
			{0.0f, 0.0f, vScale[2], 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
	}

	glm::mat4 InitRotateTransform() {
		glm::mat4 xm, ym, zm;

		float x = ToRadian(vRotate[0]);
		float y = ToRadian(vRotate[1]);
		float z = ToRadian(vRotate[2]);

		xm = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, cosf(x), -sinf(x), 0.0f},
			{0.0f, sinf(x), cosf(x), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};

		ym = {
			{cosf(y), 0.0f, -sinf(y), 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{sinf(y), 0.0f, cosf(y), 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};

		zm = {
			{cosf(z), -sinf(z), 0.0f, 0.0f},
			{sinf(z), cosf(z), 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};

		return xm * ym * zm;
	}

	glm::mat4 InitTranslationTransform() {
		return glm::mat4{
			{1.0f, 0.0f, 0.0f, vTranslation[0]},
			{0.0f, 1.0f, 0.0f, vTranslation[1]},
			{0.0f, 0.0f, 1.0f, vTranslation[2]},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};
	}

public:
	void Scale(float x, float y, float z) {
		vScale = {x, y, z};
	}

	void WorldPos(float x, float y, float z) {
		vTranslation = { x, y, z };
	}

	void Rotate(float x, float y, float z) {
		vRotate = {x, y, z};
	}

	glm::mat4 GetTrans() {
		return InitRotateTransform() * InitScaleTransform() * InitTranslationTransform();
	}
};

class GLProgram {
protected:
	GLuint shaderProgram;

public:
	GLProgram() {
		shaderProgram = glCreateProgram();
	}

	void linkProgram() {
		glLinkProgram(shaderProgram);
		GLint Success;
		GLchar* ErrorLog = nullptr;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
		if (Success == 0) {
			glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		}
	}

	void use() {
		glUseProgram(shaderProgram);
	}
};

class GLShaderProgram : public GLProgram {
	void compileShader(GLuint ShaderObj) {
		glCompileShader(ShaderObj);

		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
			fprintf(stderr, "Error compiling shader: '%s'\n", InfoLog);
		}
	}

public:
	GLuint getUniformLocation(const char* name) {
		return glGetUniformLocation(shaderProgram, (GLchar*)name);
	}

	void addShader(GLenum type, const GLchar* text) {
		GLuint ShaderObj = glCreateShader(type);
		const GLchar* p[1];
		p[0] = text;
		GLint Lengths[1];
		Lengths[0] = strlen(text);
		glShaderSource(ShaderObj, 1, p, Lengths);

		this->compileShader(ShaderObj);

		glAttachShader(shaderProgram, ShaderObj);

		glValidateProgram(shaderProgram);
	}
};



void render() {
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.1f;
	Scale += 0.001f;

	Pipeline p;
	p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	p.WorldPos(sinf(Scale), 0.0f, 0.0f);
	p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
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
	glutInitWindowSize(1024, 768);
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
