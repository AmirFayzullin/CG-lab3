#pragma once
#include "GLProgram.h"
#include <glm/glm.hpp>

const GLchar pVS[] = "\n\
#version 330\n\
layout (location = 0) in vec3 Position; \n\
layout (location = 1) in vec2 TexCoord; \n\
uniform mat4 gWorld; \n\
out vec2 TexCoord0;  \n\
void main() { \n\
	gl_Position = gWorld * vec4(Position, 1.0); \n\
	TexCoord0 = TexCoord;\n\
}";
const GLchar pFS[] = "\n\
#version 330\n\
in vec2 TexCoord0; \n\
out vec4 FragColor; \n\
struct DirectionalLight \n\
{ \n\
    vec3 Color; \n\
    float AmbientIntensity; \n\
};\n\
uniform DirectionalLight gDirectionalLight; \n\
uniform sampler2D gSampler;\n\
void main() {\n\
	FragColor = texture2D(gSampler, TexCoord0.xy) * vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity; \n\
}";

struct DirectionLight
{
	glm::vec3 Color;
	float AmbientIntensity;
};

class LightingProgram : public GLProgram {
	GLuint gWorldLocation, gSampler, dirLightColor, dirLightAmbientIntensity;
public:
	bool init() override {
		if (!GLProgram::init()) return false;

		if (!addShader(GL_VERTEX_SHADER, pVS)) return false;
		if (!addShader(GL_FRAGMENT_SHADER, pFS)) return false;

		if (!linkProgram()) return false;

		gWorldLocation = getUniformLocation("gWorld");
		gSampler = getUniformLocation("gSampler");
		dirLightColor = getUniformLocation("gDirectionalLight.Color");
		dirLightAmbientIntensity = getUniformLocation("gDirectionalLight.AmbientIntensity");

		return true;
	}

	void setGWP(const GLfloat* value) {
		glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, value);
	}

	void setTextureUnit(int textureUnit) {
		glUniform1i(gSampler, textureUnit);
	}

	void setDirectionalLight(const DirectionLight& Light) {
		glUniform3f(dirLightColor, Light.Color.x, Light.Color.y, Light.Color.z);
		glUniform1f(dirLightAmbientIntensity, Light.AmbientIntensity);
	}
};