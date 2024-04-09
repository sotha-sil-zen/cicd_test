#include <iostream>
#include <vulkan/vulkan.h>
#include <assimp/Importer.hpp>
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "Eigen/Core"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int main() 
{	
    Eigen::Vector3d x;
    stbi_image_free(nullptr);
    // 初始化Assimp导入器
    Assimp::Importer importer;
    // 初始化Vulkan实例
    VkInstance instance;
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
        return 1;
    }

    std::cout << "Vulkan instance created successfully!" << std::endl;
    ImGui::CreateContext();
    ImGui_ImplVulkan_InitInfo init_info = {};
    // 清理资源
    vkDestroyInstance(instance, nullptr);

    return 0;
}
