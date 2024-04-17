#ifndef SIL_VK_STRUCT_H
#define SIL_VK_STRUCT_H
#include "vk_utils.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <optional>
#include <array>
namespace Sil
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete();
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription();

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
    };

    struct UniformBufferObject
    {   
        alignas(4) int is_selected;
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class Camera
    {
    private:
        glm::mat4 m_viewMatrix;
        glm::vec3 m_eye; // Camera position in 3D
        glm::vec3 m_lookAt; // Point that the camera is looking at
        glm::vec3 m_upVector; // Orientation of the camera
    public:
        Camera();
        Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector);
        glm::mat4 GetViewMatrix();
        glm::vec3 GetEye();
        glm::vec3 GetUpVector();
        glm::vec3 GetLookAt();
        // Camera forward is -z
        glm::vec3 GetViewDir();
        glm::vec3 GetRightVector();
        void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);
        void UpdateViewMatrix();
    };
}
#endif