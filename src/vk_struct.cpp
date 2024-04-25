#include "../include/vk_struct.h"

namespace Sil
{
    bool QueueFamilyIndices::isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    VkVertexInputBindingDescription Vertex::getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    Camera::Camera() {}
    /*
    Camera::Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        m_eye = eye;
        m_lookAt = lookat;
        m_upVector = upVector;
        UpdateViewMatrix();
        SetPerspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
    }
    Camera::Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float orthoSize, float aspectRatio, float nearPlane, float farPlane)
    {
        m_eye = eye;
        m_lookAt = lookat;
        m_upVector = upVector;
        UpdateViewMatrix();
        SetOrthographicProjection(orthoSize, aspectRatio, nearPlane, farPlane);
    }
    */
    glm::mat4 Camera::GetProjMatrix()
    {
        return m_projectionMatrix;
    }
    glm::mat4 Camera::GetViewMatrix()
    {
        return m_viewMatrix;
    }
    glm::vec3 Camera::GetEye()
    {
        return m_eye;
    }
    glm::vec3 Camera::GetUpVector()
    {
        return m_upVector;
    }
    glm::vec3 Camera::GetLookAt()
    {
        return m_lookAt;
    }
    // Camera forward is -z
    glm::vec3 Camera::GetViewDir()
    {
        return -glm::transpose(m_viewMatrix)[2];
    }
    glm::vec3 Camera::GetRightVector()
    {
        return glm::transpose(m_viewMatrix)[0];
    }
    void Camera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
    {
        m_eye = std::move(eye);
        m_lookAt = std::move(lookat);
        m_upVector = std::move(up);
        UpdateViewMatrix();
    }
    void Camera::UpdateViewMatrix()
    {
        // Generate view matrix using the eye, lookAt and up vector
        m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
    }

    void Camera::SetPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        m_isOrtho = false;
        m_fov = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void Camera::SetOrthographicProjection(float orthoSize, float aspectRatio, float nearPlane, float farPlane)
    {
        m_isOrtho = true;
        m_orthoSize = orthoSize;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        // Calculate the orthographic projection matrix
        float halfOrthoWidth = orthoSize * aspectRatio * 0.5f;
        float halfOrthoHeight = orthoSize * 0.5f;
        m_projectionMatrix = glm::ortho(-halfOrthoWidth, halfOrthoWidth, -halfOrthoHeight, halfOrthoHeight, nearPlane, farPlane);
    }

    float Camera::getFov()
    {
        return m_fov;
    }
    float Camera::getAspectRatio()
    {
        return m_aspectRatio;
    }
    float Camera::getNearPlane()
    {
        return m_nearPlane;
    }
    float Camera::getFarPlane()
    {
        return m_farPlane;
    }
    float Camera::getOrthoSize()
    {
        return m_orthoSize;
    }

    bool Camera::IsOrthoNow()
    {
        return m_isOrtho;
    }

    void Camera::switchProjType()
    {
        if (m_isOrtho)
        {
            float orthoViewportSize = 2.0f * m_orthoSize;

            // 根据正交投影的视口大小推导透视投影的FOV
            float halfFovY = atan(orthoViewportSize / (2.0f * glm::length(m_eye))) * (180.0f / M_PI);
            float fov = 2.0f * halfFovY;
            SetPerspectiveProjection(fov, m_aspectRatio, m_nearPlane, m_farPlane);
        }
        else
        {
            float halfFovY = m_fov / 2.0f;
            float perspectiveViewportSize = 2.0f * glm::length(m_eye) * tan(glm::radians(halfFovY));

            // 根据透视投影的视口大小推导正交投影的大小
            float orthoSize = perspectiveViewportSize / 2.0f;
            SetOrthographicProjection(orthoSize, m_aspectRatio, m_nearPlane, m_farPlane);
        }
    }

    CameraManager::CameraManager()
    {
    }

    CameraManager::CameraManager(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float fov, float aspectRatio, float nearPlane, float farPlane, std::string name)
    {
        camera_inst_vec.emplace_back();
        camera_inst_vec.back().SetCameraView(eye, lookat, upVector);
        camera_inst_vec.back().SetPerspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
        camera_name_vec.emplace_back(name);
        camera_freezed_vec.emplace_back(0);
        cur_cam_idx = 0;
    }
    size_t CameraManager::getCurIdx()
    {
        return cur_cam_idx;
    }
    void CameraManager::setCurIdx(size_t idx)
    {
        cur_cam_idx = idx;
    }
    size_t CameraManager::getCount()
    {
        return camera_inst_vec.size();
    }
    const Camera &CameraManager::getCamera(size_t idx) const
    {
        return const_cast<CameraManager *>(this)->getCamera(cur_cam_idx);
    }
    const std::string &CameraManager::getName(size_t idx) const
    {
        return const_cast<CameraManager *>(this)->getName(cur_cam_idx);
    }
    const int& CameraManager::getFreezed(size_t idx) const
    {
        return const_cast<CameraManager *>(this)->getFreezed(cur_cam_idx);
    }
    const Camera &CameraManager::getCurCamera() const
    {
        return const_cast<CameraManager *>(this)->getCurCamera();
    }
    const std::string &CameraManager::getCurName() const
    {
        return const_cast<CameraManager *>(this)->getCurName();
    }
    const int& CameraManager::getCurFreezed() const
    {
        return const_cast<CameraManager *>(this)->getCurFreezed();
    }

    Camera &CameraManager::getCamera(size_t idx)
    {
        return camera_inst_vec[idx];
    }
    std::string &CameraManager::getName(size_t idx)
    {
        return camera_name_vec[idx];
    }
    int& CameraManager::getFreezed(size_t idx)
    {
        return camera_freezed_vec[idx];
    }
    Camera &CameraManager::getCurCamera()
    {
        return camera_inst_vec[cur_cam_idx];
    }
    std::string &CameraManager::getCurName()
    {
        return camera_name_vec[cur_cam_idx];
    }
    int& CameraManager::getCurFreezed()
    {
        return camera_freezed_vec[cur_cam_idx];
    }
    void CameraManager::copyCurCamera(std::string name)
    {
        Camera cam_copy = camera_inst_vec[cur_cam_idx];
        camera_inst_vec.push_back(cam_copy);
        camera_name_vec.emplace_back(name);
        camera_freezed_vec.push_back(camera_freezed_vec[cur_cam_idx]);
        cur_cam_idx = camera_inst_vec.size() - 1;
    }

    void CameraManager::setName(std::string name, size_t idx)
    {
        camera_name_vec[idx] = name;
    }
    void CameraManager::setFreezed(int freezed_int, size_t idx)
    {
        camera_freezed_vec[idx]=freezed_int;
    }
    bool CameraManager::del()
    {
        // at least 1 camera.
        if (getCount() == 1)
        {
            return false;
        }
        camera_name_vec.erase(camera_name_vec.begin() + cur_cam_idx);
        camera_inst_vec.erase(camera_inst_vec.begin() + cur_cam_idx);
        camera_freezed_vec.erase(camera_freezed_vec.begin() + cur_cam_idx);
        setCurIdx(0);
        return true;
    }
}
