#pragma once
#include "GLProgram.h"
#include <glm/glm.hpp>
#include "Math_3d.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

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
const int MAX_POINT_LIGHTS = 3;\n\
const int MAX_SPOT_LIGHTS = 2;\n\
in vec2 TexCoord0; \n\
in vec3 Normal0;\n\
in vec3 WorldPos0;\n\
out vec4 FragColor;\n\
struct BaseLight\n\
{\n\
    vec3 Color;\n\
    float AmbientIntensity;\n\
    float DiffuseIntensity;\n\
};\n\
struct Attenuation\n\
{\n\
    float Constant;\n\
    float Linear;\n\
    float Exp;\n\
};\n\
\n\
struct PointLight\n\
{\n\
    BaseLight Base;\n\
    vec3 Position;\n\
    Attenuation Atten;\n\
};\n\
struct SpotLight\n\
{\n\
    PointLight Base;\n\
    vec3 Direction;\n\
    float Cutoff;\n\
};\n\
struct DirectionalLight\n\
{\n\
    BaseLight Base;\n\
    vec3 Direction;\n\
};\n\
uniform int gNumPointLights;\n\
uniform int gNumSpotLights;\n\
uniform PointLight gPointLights[MAX_POINT_LIGHTS];\n\
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];\n\
uniform DirectionalLight gDirectionalLight; \n\
uniform sampler2D gSampler;\n\
uniform vec3 gEyeWorldPos;\n\
uniform float gSpecularIntensity;\n\
uniform float gSpecularPower;\n\
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)\n\
{\n\
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;\n\
    float DiffuseFactor = dot(Normal, -LightDirection);\n\
    \n\
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);\n\
    vec4 SpecularColor = vec4(0, 0, 0, 0);\n\
	\n\
    if (DiffuseFactor > 0) {\n\
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;\n\
		\n\
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);\n\
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));\n\
        float SpecularFactor = dot(VertexToEye, LightReflect);\n\
        SpecularFactor = pow(SpecularFactor, gSpecularPower);\n\
        if (SpecularFactor > 0) {\n\
            SpecularColor = vec4(Light.Color, 1.0f) *\n\
                            gSpecularIntensity * SpecularFactor;\n\
        }\n\
    }\n\
	\n\
    return (AmbientColor + DiffuseColor + SpecularColor);\n\
}\n\
\n\
vec4 CalcDirectionalLight(vec3 Normal)\n\
{\n\
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);\n\
}\n\
vec4 CalcPointLight( PointLight l, vec3 Normal)\n\
{\n\
    vec3 LightDirection = WorldPos0 - l.Position;\n\
    float Distance = length(LightDirection);\n\
    LightDirection = normalize(LightDirection);\n\
	\n\
    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);\n\
    float Attenuation =  l.Atten.Constant + \n\
                         l.Atten.Linear * Distance + \n\
                         l.Atten.Exp * Distance * Distance;\n\
	\n\
    return Color / Attenuation;\n\
}\n\
\n\
vec4 CalcSpotLight( SpotLight l, vec3 Normal)\n\
{\n\
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);\n\
    float SpotFactor = dot(LightToPixel, l.Direction);\n\
    \n\
    if (SpotFactor > l.Cutoff) {\n\
        vec4 Color = CalcPointLight(l.Base, Normal);\n\
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));\n\
    }\n\
    else {\n\
        return vec4(0,0,0,0);\n\
    }\n\
}\n\
void main()\n\
{\n\
    vec3 Normal = normalize(Normal0);\n\
    vec4 TotalLight = CalcDirectionalLight(Normal);\n\
	\n\
    for (int i = 0 ; i < gNumPointLights ; i++) {\n\
        TotalLight += CalcPointLight(gPointLights[i], Normal);\n\
    }\n\
	\n\
    for (int i = 0 ; i < gNumSpotLights ; i++) {\n\
        TotalLight += CalcSpotLight(gSpotLights[i], Normal);\n\
    }\n\
	\n\
    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;\n\
}";

struct DirectionalLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	glm::vec3 Direction;
	float DiffuseIntensity;
};

struct PointLightLocation {
	GLuint Color;
	GLuint AmbientIntensity;
	GLuint DiffuseIntensity;
	GLuint Position;
	struct {
		GLuint Constant;
		GLuint Linear;
		GLuint Exp;
	} Atten;
};

struct SpotLightLocation {
	GLuint Color;
	GLuint AmbientIntensity;
	GLuint DiffuseIntensity;
	GLuint Position;
	GLuint Direction;
	GLuint Cutoff;
	struct {
		GLuint Constant;
		GLuint Linear;
		GLuint Exp;
	} Atten;
};

struct BaseLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = glm::vec3{ 0.0f, 0.0f, 0.0f };
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
};


struct PointLight : public BaseLight
{
	glm::vec3 Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = glm::vec3{ 0.0f, 0.0f, 0.0f };
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = glm::vec3{ 0.0f, 0.0f, 0.0f };
		Cutoff = 0.0f;
	}
};

class LightingProgram : public GLProgram {
	GLuint gWVP, worldMatrix, gSampler,
		dirLightColor, dirLightAmbientIntensity,
		dirLightDirection, dirLightDiffuseIntensity,
		eyeWorldPos, specularIntensity, specularPower,
		numPointLightsLocation, numSpotLightsLocation;

	PointLightLocation pointLightsLocation[3];
	SpotLightLocation spotLightsLocation[2];

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

		numPointLightsLocation = getUniformLocation("gNumPointLights");
		numSpotLightsLocation = getUniformLocation("gNumSpotLights");

		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(pointLightsLocation); i++) {
			char Name[128];
			memset(Name, 0, sizeof(Name));
			snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
			pointLightsLocation[i].Color = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
			pointLightsLocation[i].AmbientIntensity = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
			pointLightsLocation[i].Position = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
			pointLightsLocation[i].DiffuseIntensity = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
			pointLightsLocation[i].Atten.Constant = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
			pointLightsLocation[i].Atten.Linear = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
			pointLightsLocation[i].Atten.Exp = getUniformLocation(Name);
		}


		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(spotLightsLocation); i++) {
			char Name[128];
			memset(Name, 0, sizeof(Name));
			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
			spotLightsLocation[i].Color = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
			spotLightsLocation[i].AmbientIntensity = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
			spotLightsLocation[i].Position = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
			spotLightsLocation[i].Direction = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
			spotLightsLocation[i].Cutoff = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
			spotLightsLocation[i].DiffuseIntensity = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
			spotLightsLocation[i].Atten.Constant = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
			spotLightsLocation[i].Atten.Linear = getUniformLocation(Name);

			snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
			spotLightsLocation[i].Atten.Exp = getUniformLocation(Name);
		}
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

	void setPointLights(unsigned int NumLights, const PointLight* pLights) {
		glUniform1i(numPointLightsLocation, NumLights);

		for (unsigned int i = 0; i < NumLights; i++) {
			glUniform3f(pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
			glUniform1f(pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
			glUniform1f(pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
			glUniform3f(pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
			glUniform1f(pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
			glUniform1f(pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
			glUniform1f(pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
		}
	}

	void setSpotLights(unsigned int NumLights, const SpotLight* pLights)
	{
		glUniform1i(numSpotLightsLocation, NumLights);

		for (unsigned int i = 0; i < NumLights; i++) {
			glUniform3f(spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
			glUniform1f(spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
			glUniform1f(spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
			glUniform3f(spotLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
			glm::vec3 Direction = pLights[i].Direction;
			VectorsMath::Normalize(Direction);
			glUniform3f(spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
			glUniform1f(spotLightsLocation[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
			glUniform1f(spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
			glUniform1f(spotLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
			glUniform1f(spotLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
		}
	}

};