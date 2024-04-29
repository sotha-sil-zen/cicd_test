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
    struct GridUniformBufferObject
    {
        alignas(4) float grid_width;
        alignas(4) float grid_space;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class Camera
    {
    private:
        glm::mat4 m_viewMatrix;
        glm::vec3 m_eye;      // Camera position in 3D
        glm::vec3 m_lookAt;   // Point that the camera is looking at
        glm::vec3 m_upVector; // Orientation of the camera

        glm::mat4 m_projectionMatrix;
        float m_fov;         // Field of view for perspective projection
        float m_aspectRatio; // Aspect ratio for perspective projection
        float m_nearPlane;   // Near plane distance for perspective projection
        float m_farPlane;    // Far plane distance for perspective projection
        float m_orthoSize;   // Size for orthographic projection
        bool m_isOrtho;      // Flag to indicate if current projection is orthographic
    public:
        Camera();
        // Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float fov, float aspectRatio, float nearPlane, float farPlane);
        // Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float orthoSize, float aspectRatio, float nearPlane, float farPlane);
        glm::mat4 GetProjMatrix();
        glm::mat4 GetViewMatrix();
        glm::vec3 GetEye();
        glm::vec3 GetUpVector();
        glm::vec3 GetLookAt();
        // Camera forward is -z
        glm::vec3 GetViewDir();
        glm::vec3 GetRightVector();
        void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);
        void UpdateViewMatrix();
        // Setters for perspective projection parameters
        void SetPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
        // Setters for orthographic projection parameters
        void SetOrthographicProjection(float orthoSize, float aspectRatio, float nearPlane, float farPlane);
        float getFov();
        float getAspectRatio();
        float getNearPlane();
        float getFarPlane();
        float getOrthoSize();
        bool IsOrthoNow();
        void switchProjType();
    };

    // 0 for unfreezed, 1 for freezed.
    class CameraManager
    {
    private:
        std::vector<Camera> camera_inst_vec;
        std::vector<std::string> camera_name_vec;
        std::vector<int> camera_freezed_vec;
        size_t cur_cam_idx;

    public:
        CameraManager();
        CameraManager(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector, float fov, float aspectRatio, float nearPlane, float farPlane, std::string name);
        size_t getCurIdx();
        void setCurIdx(size_t idx);
        size_t getCount();
        const Camera &getCamera(size_t idx) const;
        const std::string &getName(size_t idx) const;
        // std::vector<bool> has specilizition of std::vector<T> , operator[] return a copy ,not ref.
        const int &getFreezed(size_t idx) const;
        const Camera &getCurCamera() const;
        const std::string &getCurName() const;
        const int &getCurFreezed() const;

        Camera &getCamera(size_t idx);
        std::string &getName(size_t idx);
        int &getFreezed(size_t idx);
        Camera &getCurCamera();
        std::string &getCurName();
        int &getCurFreezed();
        void copyCurCamera(std::string name);
        void setName(std::string name, size_t idx);
        void setFreezed(int freezed_int, size_t idx);
        bool del();
    };

    class Gizmo
    {
    public:
        enum class GizmoType
        {
            NONE,
            TRANSLATE,
            SCALE,
            ROTATE,
        };
        enum class GizmoAction
        {
            NONE,
            MOVE_X,
            MOVE_Y,
            MOVE_Z,
            MOVE_YZ,
            MOVE_XZ,
            MOVE_XY,
            MOVE_XYZ,
            ROTATE_X,
            ROTATE_Y,
            ROTATE_Z,
            SCALE_X,
            SCALE_Y,
            SCALE_Z,
            SCALE_XYZ,
        };

        Gizmo();

    private:
        struct GizmoRay
        {
            glm::vec3 origin;
            glm::vec3 dirction;
        };
        GizmoType type;
        GizmoAction action;
        bool local_toggle;
        std::vector<Vertex> vertex_vec;
        std::vector<uint32_t> facet_vec;
        // static bool intersect_ray_plane(const GizmoRay& ray, const )
        static std::pair<bool, float> rayIntersectTriangle(const GizmoRay &ray, const Vertex &v0, const Vertex &v1, const Vertex &v2);
        std::pair<bool, float> rayIntersectGizmoMesh(const GizmoRay &ray);
        void makeBox(const glm::vec3 &min_bounds, const glm::vec3 &max_bounds);
        void makeLathed(const glm::vec3 &axis, const glm::vec3 &arm1, const glm::vec3 &arm2, int slices, const std::vector<glm::vec2> &points, const float eps = 0.0f);
        void switchType();
    };
}
#endif