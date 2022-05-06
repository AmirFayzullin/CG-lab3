//#pragma once
//#include <glm/glm.hpp>
//#include "Math_3d.h"
//
//#define M_PI 3.14159265358979323846
//#define ToRadian(x) ((x) * M_PI / 180.0f)
//#define ToDegree(x) ((x) * 180.0f / M_PI)
//
//
//
//class Pipeline {
//	struct PerspectiveProjInfo {
//		float FOV;
//		float Width;
//		float Height;
//		float zNear;
//		float zFar;
//	};
//
//	struct {
//		glm::vec3 Pos;
//		glm::vec3 Target;
//		glm::vec3 Up;
//	} m_camera;
//
//	const int x = 0, y = 1, z = 2;
//
//	glm::vec3
//		vScale{ 1.0f, 1.0f, 1.0f },
//		vRotate{ 0.0f, 0.0f, 0.0f },
//		vTranslation{ 0.0f, 0.0f, 0.0f };
//
//	glm::mat4 worldTransformation, WVPTransformation;
//
//	PerspectiveProjInfo perspectiveProjInfo{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
//
//	glm::mat4 InitScaleTransform(float x, float y, float z) {
//		return glm::mat4{
//			{x, 0.0f, 0.0f, 0.0f},
//			{0.0f, y, 0.0f, 0.0f},
//			{0.0f, 0.0f, z, 0.0f},
//			{0.0f, 0.0f, 0.0f, 1.0f}
//		};
//	}
//
//	glm::mat4 InitRotateTransform(float _x, float _y, float _z) {
//		glm::mat4 xm, ym, zm;
//
//		float x = ToRadian(_x);
//		float y = ToRadian(_y);
//		float z = ToRadian(_z);
//
//		xm = {
//			{1.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, cosf(x), -sinf(x), 0.0f},
//			{0.0f, sinf(x), cosf(x), 0.0f},
//			{0.0f, 0.0f, 0.0f, 1.0f},
//		};
//
//		ym = {
//			{cosf(y), 0.0f, -sinf(y), 0.0f},
//			{0.0f, 1.0f, 0.0f, 0.0f},
//			{sinf(y), 0.0f, cosf(y), 0.0f},
//			{0.0f, 0.0f, 0.0f, 1.0f},
//		};
//
//		zm = {
//			{cosf(z), -sinf(z), 0.0f, 0.0f},
//			{sinf(z), cosf(z), 0.0f, 0.0f},
//			{0.0f, 0.0f, 1.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 1.0f},
//		};
//
//		return xm * ym * zm;
//	}
//
//	glm::mat4 InitTranslationTransform(float x, float y, float z) {
//		return glm::mat4{
//			{1.0f, 0.0f, 0.0f, x},
//			{0.0f, 1.0f, 0.0f, y},
//			{0.0f, 0.0f, 1.0f, z},
//			{0.0f, 0.0f, 0.0f, 1.0f},
//		};
//	}
//
//	glm::mat4 InitPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar) {
//		const float ar = Width / Height;
//		const float zRange = zNear - zFar;
//		const float tanHalfFOV = tanf(ToRadian(FOV / 2.0));
//
//		return glm::mat4{
//			{1.0f / (ar * tanHalfFOV), 0.0f, 0.0f, 0.0f},
//			{0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f},
//			{0.0f, 0.0f, (-zNear - zFar) / zRange, (2.0f * zFar * zNear) / zRange},
//			{0.0f, 0.0f, 1.0f, 0.0f}
//		};
//	}
//
//	glm::mat4 InitCameraTransform(glm::vec3 Target, glm::vec3 Up) {
//		glm::vec3 N = glm::vec3(Target);
//		VectorsMath::Normalize(N);
//		glm::vec3 _U = glm::vec3(Up);
//		VectorsMath::Normalize(_U);
//		glm::vec3 U = VectorsMath::Cross(_U, Target);
//		glm::vec3 V = VectorsMath::Cross(N, U);
//
//		return glm::mat4{
//			{U[x], U[y], U[z], 0.0f},
//			{V[x], V[y], V[z], 0.0f},
//			{N[x], N[y], N[z], 0.0f},
//			{0.0f, 0.0f, 0.0f, 1.0f},
//		};
//	}
//
//public:
//	void Scale(float x, float y, float z) {
//		vScale = { x, y, z };
//	}
//
//	void WorldPos(float x, float y, float z) {
//		vTranslation = { x, y, z };
//	}
//
//	void Rotate(float x, float y, float z) {
//		vRotate = { x, y, z };
//	}
//
//	void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar) {
//		perspectiveProjInfo = { FOV, Width, Height, zNear, zFar };
//	}
//
//	void SetCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
//		m_camera = { pos, target, up };
//	}
//
//	glm::mat4 GetWorldTrans() {
//		glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;
//
//		ScaleTrans = InitScaleTransform(vScale[x], vScale[y], vScale[z]);
//		RotateTrans = InitRotateTransform(vRotate[x], vRotate[y], vRotate[z]);
//		TranslationTrans = InitTranslationTransform(vTranslation[x], vTranslation[y], vTranslation[z]);
//
//		return ScaleTrans * RotateTrans * TranslationTrans;
//	}
//
//	glm::mat4 GetWVPTrans() {
//		GetWorldTrans();
//		glm::mat4 CameraTranslationTrans, CameraRotateTrans, PersProjTrans;
//
//		CameraTranslationTrans = InitTranslationTransform(-m_camera.Pos[x], -m_camera.Pos[y], -m_camera.Pos[z]);
//		CameraRotateTrans = InitCameraTransform(m_camera.Target, m_camera.Up);
//		PersProjTrans = InitPerspectiveProj(perspectiveProjInfo.FOV, perspectiveProjInfo.Width, perspectiveProjInfo.Height, perspectiveProjInfo.zNear, perspectiveProjInfo.zFar);
//
//		WVPTransformation = worldTransformation;
//
//		return WVPTransformation;
//	}
//};

# define MATH_PI 3.14159265358979323846


#pragma once
#include <glm/glm.hpp>

class PipelineHandler
{
public:
    PipelineHandler();

    void setPosition(float x, float y, float z);
    void setRotation(int degX, int degY, int degZ);
    void setScale(float x, float y, float z);

    void setPerspective(float fov,
        float width, float height,
        float zNear, float zFar);
    void setCamera(const glm::vec3& pos,
        const glm::vec3& target,
        const glm::vec3& up);

    glm::mat4* getTransformationMatrix();

private:
    glm::mat4 getTranslationTransformation(float x, float y, float z);
    glm::mat4 getRotationTransformation();
    glm::mat4 getScaleTransformation();
    glm::mat4 getProjectionTransformation();
    glm::mat4 getCameraRotationTransformation(
        const glm::vec3& targetVec,
        const glm::vec3& upVec);

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    struct ProjectionProps
    {
        float fov;
        float width;
        float height;
        float zNear;
        float zFar;
    };
    ProjectionProps m_projection;

    struct CameraProps
    {
        glm::vec3 posVec;
        glm::vec3 targetVec;
        glm::vec3 upVec;
    };
    CameraProps m_camera;

    glm::mat4 m_transformation;
};

class Pipeline {
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    struct ProjectionProps
    {
        float fov;
        float width;
        float height;
        float zNear;
        float zFar;
    };
    ProjectionProps m_projection;

    struct CameraProps
    {
        glm::vec3 posVec;
        glm::vec3 targetVec;
        glm::vec3 upVec;
    };
    CameraProps m_camera;

    glm::mat4 m_transformation;

    glm::mat4 getTranslationTransformation(float x, float y, float z)
    {
        return glm::mat4({
            {1.0f,0.0f,0.0f,x},
            {0.0f,1.0f,0.0f,y},
            {0.0f,0.0f,1.0f,z},
            {0.0f,0.0f,0.0f,1.0f}
            });
    }
    glm::mat4 getRotationTransformation()
    {
        double x = ((m_rotation.x) * MATH_PI / 180.0f);
        double y = ((m_rotation.y) * MATH_PI / 180.0f);
        double z = ((m_rotation.z) * MATH_PI / 180.0f);

        glm::mat4 rotationX = {
            {1.0f,  0.0f,   0.0f,       0.0f},
            {0.0f,  cos(x), -sin(x),    0.0f},
            {0.0f,  sin(x), cos(x),     0.0f},
            {0.0f,  0.0f,   0.0f,       1.0f}
        };

        glm::mat4 rotationY = {
            {cos(y),  0.0f,   -sin(y),  0.0f},
            {0.0f,    1.0f,   0.0f,     0.0f},
            {sin(y),  0.0f,   cos(y),   0.0f},
            {0.0f,    0.0f,   0.0f,     1.0f}
        };

        glm::mat4 rotationZ = {
            {cos(z), -sin(z),    0.0f,   0.0f},
            {sin(z),  cos(z),    0.0f,   0.0f},
            {0.0f,      0.0f,   1.0f,   0.0f},
            {0.0f,      0.0f,   0.0f,   1.0f}
        };

        return rotationX * rotationY * rotationZ;
    }
    glm::mat4 getScaleTransformation()
    {
        return glm::mat4({
            {m_scale.x,0.0f,0.0f,0.0f},
            {0.0f,m_scale.y,0.0f,0.0f},
            {0.0f,0.0f,m_scale.z,0.0f},
            {0.0f,0.0f,0.0f,     1.0f}
            });
    }
    glm::mat4 getProjectionTransformation()
    {
        const float ar = m_projection.width / m_projection.height;
        const float zNear = m_projection.zNear;
        const float zFar = m_projection.zFar;
        const float zRange = zNear - zFar;
        const float tanHalfFOV = glm::tan(glm::radians(m_projection.fov) / 2.0);

        glm::mat4 matrix = glm::mat4({
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f}
            });

        matrix[0][0] = 1.0f / (tanHalfFOV * ar);

        matrix[1][1] = 1.0f / tanHalfFOV;
        matrix[2][2] = (-zNear - zFar) / zRange;
        matrix[2][3] = 2.0f * zFar * zNear / zRange;

        return matrix;
    }
    glm::mat4 getCameraRotationTransformation(
        const glm::vec3& targetVec,
        const glm::vec3& upVec)
    {
        glm::vec3 N = targetVec;
        N = glm::normalize(N);
        glm::vec3 U = upVec;
        U = glm::normalize(U);
        U = glm::cross(U, targetVec);
        glm::vec3 V = glm::cross(N, U);


        glm::mat4 matrix = glm::mat4({
            {U.x,  U.y,  U.z,  0.0f},
            {V.x,  V.y,  V.z,  0.0f},
            {N.x,  N.y,  N.z,  0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
            });
        return matrix;
    }

    public:

    Pipeline()
    {
        m_scale = { 1.0f, 1.0f, 1.0f };
        m_position = { 0.0f, 0.0f, 0.0f };
        m_rotation = { 0.0f, 0.0f, 0.0f };
    }

    glm::mat4 getTransformationMatrix()
    {
        glm::mat4 translationTransformation = getTranslationTransformation(
            m_position.x,
            m_position.y,
            m_position.z);
        glm::mat4 rotationTransformation = getRotationTransformation();
        glm::mat4 scaleTransformation = getScaleTransformation();

        glm::mat4 projectionTransformation = getProjectionTransformation();

        glm::mat4 cameraTranslation = getTranslationTransformation(
            -m_camera.posVec.x,
            -m_camera.posVec.y,
            -m_camera.posVec.z);
        glm::mat4 cameraRotation = getCameraRotationTransformation(
            m_camera.targetVec,
            m_camera.upVec);

        m_transformation = //projectionTransformation *
            //cameraRotation *
            scaleTransformation *
            rotationTransformation *
            translationTransformation;
        //cameraTranslation;

        return m_transformation;
    }

    void WorldPos(float x, float y, float z)
    {
        m_position.x = x;
        m_position.y = y;
        m_position.z = z;
    }
    void Rotate(int x, int y, int z)
    {
        m_rotation.x = x;
        m_rotation.y = y;
        m_rotation.z = z;
    }
    void Scale(float x, float y, float z)
    {
        m_scale.x = x;
        m_scale.y = y;
        m_scale.z = z;
    }
    void SetPerspectiveProj(float fov,
        float width, float height,
        float zNear, float zFar)
    {
        m_projection.fov = fov;
        m_projection.width = width;
        m_projection.height = height;
        m_projection.zNear = zNear;
        m_projection.zFar = zFar;
    }
    void SetCamera(
        const glm::vec3& pos,
        const glm::vec3& target,
        const glm::vec3& up)
    {
        m_camera.posVec = pos;
        m_camera.targetVec = target;
        m_camera.upVec = up;
    }
};