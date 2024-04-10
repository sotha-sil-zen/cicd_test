#ifndef TMESH_H_
#define TMESH_H_
#include <Eigen/Core>
#include <vulkan/vulkan.h>
#include <filesystem>
#include <optional>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
namespace Sil
{
    struct TMesh
    {
        Eigen::MatrixXd _vertices;
        Eigen::MatrixXi _faces;
        Eigen::MatrixXd _tex_coords;
        Eigen::MatrixXd _normals;

        int _texture_id;
    };

    class TextureData
    {
        friend class TextureImporter;

    public:
        struct ImageData
        {
            uint32_t _width = 0;
            uint32_t _height = 0;
            uint32_t _channels = 0;
            unsigned char *_data = nullptr;
            ~ImageData();
        };
        TextureData(const std::filesystem::path &path);
        TextureData(TextureData &&texture);
        std::optional<ImageData> _data;
    };

    enum class FileType
    {
        kUnknown = 0,
        kImage = 1,
        kModel = 2,
    };

    FileType inferFiletype(const std::filesystem::path &filepath);
    /*
    如果要将这些句柄放入std::vector中，建议你在管理资源的同时也要考虑正确的生命周期管理。以下是一些注意事项：

    正确释放资源： 在从std::vector中删除句柄时，确保调用Vulkan函数来正确地释放相应的资源。例如，对于VkImageView，你需要调用vkDestroyImageView来销毁对应的视图。

    确保生命周期一致性： 确保你不会在资源尚未完成使用的情况下从std::vector中删除句柄。这可能需要对句柄的使用进行适当的同步，以确保它们不会在使用时被释放。

    防止悬空句柄： 确保在删除句柄后，将其置为无效，以防止之后无意中使用悬空句柄。

    避免拷贝和移动问题： Vulkan的句柄通常不支持拷贝和移动语义，所以在使用std::vector时，需要小心处理复制和移动操作。
    */
    class TMeshManager
    {
    private:
        std::vector<TMesh> _meshes;

        std::vector<size_t> _mesh_use_texture_idx;
        std::vector<size_t> _mesh_use_ubo_idx;

        std::vector<TextureData> _textures;

        std::vector<VkImage> _textureImages;
        std::vector<VkImageView> _textureImagesView;
        std::vector<VkDeviceMemory> _textureImagesMemory;

        std::vector<VkBuffer> _vertexBuffers;
        std::vector<VkDeviceMemory> _vertexBuffersMemory;
        std::vector<VkBuffer> _indexBuffers;
        std::vector<VkDeviceMemory> _indexBuffersMemory;

        std::vector<std::vector<VkBuffer>> _uniformBuffers;
        std::vector<std::vector<VkDeviceMemory>> _uniformBuffersMemory;
        std::vector<std::vector<void *>> _uniformBuffersMapped;

        std::vector<std::vector<VkDescriptorSet>> _descriptorSets;

        std::vector<bool>_mesh_selected;

        int _last_selected_idx;

        void addMesh(std::filesystem::path mesh_path);
        void addTexture(std::filesystem::path texture_path);

        friend class VulkInterface;

    public:
        TMeshManager();
        void addResource(std::filesystem::path path);
    };
}

#endif