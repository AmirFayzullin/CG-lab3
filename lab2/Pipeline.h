#pragma once
#include <glm/glm.hpp>

#define M_PI 3.14159265358979323846 
#define ToRadian(x) ((x) * M_PI / 180.0f) 
#define ToDegree(x) ((x) * 180.0f / M_PI) 

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

    glm::mat4 world_transformation;
    glm::mat4 WVP_transformation;

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

        double x = ToRadian(m_rotation.x);
        double y = ToRadian(m_rotation.y);
        double z = ToRadian(m_rotation.z);

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
        const glm::vec3 targetVec,
        const glm::vec3 upVec)
    {
        glm::vec3 N = glm::normalize(targetVec);

        glm::vec3 _U = glm::normalize(upVec);
        glm::vec3 U = glm::cross(_U, targetVec);

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

    glm::mat4 getWorldTrans() {
        glm::mat4 translationTransformation = getTranslationTransformation(
            m_position.x,
            m_position.y,
            m_position.z);
        glm::mat4 rotationTransformation = getRotationTransformation();
        glm::mat4 scaleTransformation = getScaleTransformation();

        world_transformation = scaleTransformation * rotationTransformation * translationTransformation;

        return world_transformation;
    }

    glm::mat4 getWVPTrans() {
        getWorldTrans();

        glm::mat4 projectionTransformation = getProjectionTransformation();

        glm::mat4 cameraTranslation = getTranslationTransformation(
            -m_camera.posVec.x,
            -m_camera.posVec.y,
            -m_camera.posVec.z);
        glm::mat4 cameraRotation = getCameraRotationTransformation(
            m_camera.targetVec,
            m_camera.upVec);

        WVP_transformation = projectionTransformation * cameraTranslation * cameraRotation * world_transformation;

        return WVP_transformation;
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
        const glm::vec3 pos,
        const glm::vec3 target,
        const glm::vec3 up)
    {
        m_camera.posVec = pos;
        m_camera.targetVec = target;
        m_camera.upVec = up;
    }
};

