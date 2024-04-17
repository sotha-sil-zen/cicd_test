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
    Camera::Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector)
    {
        m_eye = eye;
        m_lookAt = lookat;
        m_upVector = upVector;
        UpdateViewMatrix();
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
}