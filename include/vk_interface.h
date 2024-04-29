#ifndef SIL_VK_INTERFACE_H
#define SIL_VK_INTERFACE_H
#include "vk_utils.h"
#include "vk_struct.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include "ImGuiFileDialog.h"

#include "tmesh.h"

#include <cstring>
#include <set>
#include <algorithm>
#include <fstream>
#include <chrono>
namespace Sil
{   
    const std::vector<Vertex> vertices = {
        /*
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        */
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

    const std::vector<uint16_t> indices = 
    {
        0, 1, 2, 2, 3, 0,
        //4, 5, 6, 6, 7, 4
        };
        
    class VulkInterface
    {
    public:
        VulkInterface()
        {

        }
        void run()
        {
            initWindow(800, 600);
            initVulkan();
            mainLoop();
            cleanup();
        }
        void mainLoop();
    private:

        GLFWwindow *window;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        TMeshManager _mesh_manager;

        VkDescriptorPool imguiDescriptorPool;


        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        VkPipeline debugPipeline;
        VkPipelineLayout gridPipelineLayout;
        VkDescriptorSetLayout gridDescriptorSetLayout;
        VkPipeline gridPipeline;

        VkCommandPool commandPool;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImagesMemory;
        std::vector<VkImageView> depthImagesView;

        VkImage textureImage;
        VkImageView textureImageView;
        VkSampler textureSampler;
        VkDeviceMemory textureImageMemory;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;

        std::vector<VkBuffer> GridUniformBuffers;
        std::vector<VkDeviceMemory> GridUniformBuffersMemory;
        std::vector<void *> GridUniformBuffersMapped;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        VkDescriptorPool gridDescriptorPool;
        std::vector<VkDescriptorSet> gridDescriptorSets;

        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        CameraManager camera_manager;
        char camera_outliner_input_buffer[128] = { 0 };

        uint32_t currentFrame = 0;

        bool framebufferResized = false;

        double last_mouse_pos_x;
        double last_mouse_pos_y;
        bool is_camera_rorating = false;
        bool is_camera_moving=false;

        bool draw_grid=true;
        float grid_space=1.0;
        float grid_width=0.01;

        void static mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        void static mousePosCallback(GLFWwindow* window, double xpos, double ypos);
        void static mouseScrollCallback(GLFWwindow* window, double xpos, double ypos);
        void static framebufferResizeCallback(GLFWwindow *window, int width, int height);
        std::vector<char> static readFile(const std::string &filename);
        VkShaderModule createShaderModule(const std::vector<char> &code);

        void initWindow(const int &init_window_width, const int &init_window_height);
        void initVulkan();
        std::vector<const char *> getRequiredExtensions();
        bool checkValidationLayerSupport();
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        void createLogicalDevice();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
        void createSwapChain();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createImageViews();
        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createGridPipeline();

        void createDebugPipeline();

        void createRenderPass();
        void createFramebuffers();
        void createCommandPool();
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        void createDepthResources();
        void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);

        void createVertexBuffer();
        void createIndexBuffer();
        void createCamera();

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createCommandBuffers();
        void createSyncObjects();

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        // void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        


        void mapMeshToVulKan();
        void mapTextureToVulkan();
        void updateMeshToVulkan(std::string mesh_path);
        void updateTextureToVulkan(std::string texture_path);
        void recreateSwapChain();
        void cleanupSwapChain();
        void cleanup();

        void imguiDraw();
        void imguiShowMeshOutliner();
        void imguiShowCameraOutliner();
    };
}
#endif
