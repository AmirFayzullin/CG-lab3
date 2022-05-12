#pragma once
#include "GLProgram.h"
#include <glm/glm.hpp>

const GLchar pVS[] = "\n\
#version 330\n\
layout (location = 0) in vec3 Position; \n\
layout (location = 1) in vec2 TexCoord; \n\
layout (location = 2) in vec3 Normal;\n\
uniform mat4 gWVP; \n\
uniform mat4 gWorld;\n\
\n\
out vec2 TexCoord0;  \n\
out vec3 Normal0;\n\
out vec3 WorldPos0;\n\
\n\
void main() { \n\
	gl_Position = gWVP * vec4(Position, 1.0); \n\
	TexCoord0 = TexCoord;\n\
	Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;\n\
	WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;\n\
}";

const GLchar pFS[] = "\n\
#version 330\n\
in vec2 TexCoord0; \n\
in vec3 Normal0;\n\
in vec3 WorldPos0;\n\
out vec4 FragColor; \n\
struct DirectionalLight \n\
{ \n\
    vec3 Color; \n\
    float AmbientIntensity; \n\
	vec3 Direction;\n\
	float DiffuseIntensity;\n\
};\n\
uniform DirectionalLight gDirectionalLight; \n\
uniform sampler2D gSampler;\n\
uniform vec3 gEyeWorldPos;\n\
uniform float gSpecularIntensity;\n\
uniform float gSpecularPower;\n\
void main() {\n\
	vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *\n\
                        gDirectionalLight.AmbientIntensity;\n\
	vec3 LightDirection = -gDirectionalLight.Direction;\n\
    vec3 Normal = normalize(Normal0);\n\
    float DiffuseFactor = dot(Normal, LightDirection);\n\
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);\n\
    vec4 SpecularColor = vec4(0, 0, 0, 0);\n\
    if (DiffuseFactor > 0){\n\
        DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *\n\
                       gDirectionalLight.DiffuseIntensity *\n\
                       DiffuseFactor;\n\
		vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                     \n\
		vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));\n\
        float SpecularFactor = dot(VertexToEye, LightReflect);                      \n\
        SpecularFactor = pow(SpecularFactor, gSpecularPower);                       \n\
                                                                                    \n\
        if (SpecularFactor > 0){                                                    \n\
            SpecularColor = vec4(gDirectionalLight.Color, 1.0f) *                   \n\
                            gSpecularIntensity *                                 \n\
                            SpecularFactor;                                         \n\
        }                                                                           \n\
    }                                                                               \n\
    FragColor = texture2D(gSampler, TexCoord0.xy) *\n\
                (AmbientColor + DiffuseColor + SpecularColor);\n\
}";

struct DirectionalLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	glm::vec3 Direction;
	float DiffuseIntensity;
};

class LightingProgram : public GLProgram {
	GLuint gWVP, worldMatrix, gSampler, 
			dirLightColor, dirLightAmbientIntensity, 
			dirLightDirection, dirLightDiffuseIntensity,
			eyeWorldPos, specularIntensity, specularPower;
public:
	bool init() override {
		if (!GLProgram::init()) return false;

		if (!addShader(GL_VERTEX_SHADER, pVS)) return false;
		if (!addShader(GL_FRAGMENT_SHADER, pFS)) return false;

		if (!linkProgram()) return false;

		gWVP = getUniformLocation("gWVP");
		worldMatrix = getUniformLocation("gWorld");
		gSampler = getUniformLocation("gSampler");
		
		dirLightColor = getUniformLocation("gDirectionalLight.Color");
		dirLightAmbientIntensity = getUniformLocation("gDirectionalLight.AmbientIntensity");
		dirLightDirection = getUniformLocation("gDirectionalLight.Direction");
		dirLightDiffuseIntensity = getUniformLocation("gDirectionalLight.DiffuseIntensity");
		
		eyeWorldPos = getUniformLocation("gEyeWorldPos");
		specularIntensity = getUniformLocation("gSpecularIntensity");
		specularPower = getUniformLocation("gSpecularPower");

		return true;
	}

	void setGWP(const glm::mat4& value) {
		glUniformMatrix4fv(gWVP, 1, GL_TRUE, (const GLfloat*)&value[0][0]);
	}

	void setWVP(const glm::mat4& value)
	{
		glUniformMatrix4fv(worldMatrix, 1, GL_TRUE, (const GLfloat*)&value[0][0]);
	}

	void setTextureUnit(int textureUnit) {
		glUniform1i(gSampler, textureUnit);
	}

	void setDirLight(const DirectionalLight& Light) {
		glUniform3f(dirLightColor, Light.Color.x, Light.Color.y, Light.Color.z);
		glUniform1f(dirLightAmbientIntensity, Light.AmbientIntensity);
		glm::vec3 Direction = Light.Direction;
		glm::normalize(Direction);
		glUniform3f(dirLightDirection, Direction.x, Direction.y, Direction.z);
		glUniform1f(dirLightDiffuseIntensity, Light.DiffuseIntensity);
	}

	void setSpecularIntensity(float intensity) {
		glUniform1f(specularIntensity, intensity);
	}

	void setSpecularPower(float power) {
		glUniform1f(specularPower, power);
	}

	void setEyeWorldPos(const glm::vec3& _eyeWorldPos) {
		glUniform3f(eyeWorldPos, _eyeWorldPos.x, _eyeWorldPos.y, _eyeWorldPos.z);
	}
};