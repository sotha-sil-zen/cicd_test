#include "../include/tmesh.h"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include<iostream>
namespace Sil
{
    FileType inferFiletype(const std::filesystem::path &filepath)
    {
        std::string extension = filepath.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        if (extension == ".png")
            return FileType::kImage;
        if (extension == ".jpg")
            return FileType::kImage;
        if (extension == ".dae")
            return FileType::kModel;
        if (extension == ".blend")
            return FileType::kModel;
        if (extension == ".fbx")
            return FileType::kModel;
        if (extension == ".obj")
            return FileType::kModel;
        return FileType::kUnknown;
    }

    TextureData::ImageData::~ImageData()
    {
        if(_data!=nullptr)
        {
            stbi_image_free(_data);
            _data = nullptr;
        }
    }

    TextureData::TextureData(TextureData&& texture)
    {   
        this->_data.reset();
        this->_data.emplace();
        this->_data.value()._data=texture._data.value()._data;
        this->_data.value()._height=texture._data.value()._height;
        this->_data.value()._width=texture._data.value()._width;
        this->_data.value()._channels=texture._data.value()._channels;
        texture._data.value()._data=nullptr;
    }

    TextureData::TextureData(const std::filesystem::path &path)
    {
        int x, y, n;
        unsigned char *tex_data = stbi_load(path.string().c_str(), &x, &y, &n, STBI_rgb_alpha);
        if (tex_data != nullptr)
        {
            _data.emplace();
            _data.value()._width=x;
            _data.value()._height=y;
            _data.value()._data = tex_data;
            _data.value()._channels = 4;
        }
    }

    TMeshManager::TMeshManager()
    {
    }

    void TMeshManager::addResource(std::filesystem::path path)
    {
        FileType type = inferFiletype(path);
        //ASSERT(type != FileType::kUnknown, "Unknown file type!");
        std::string path_string = path.string();
        std::string stem_string = path.stem().string();
        switch (type)
        {
        case FileType::kImage:
        {
            addTexture(path);
            break;
        }
        case FileType::kModel:
        {
            addMesh(path);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void TMeshManager::addMesh(std::filesystem::path path)
    {
        Assimp::Importer *importer = new Assimp::Importer();
        const aiScene *ai_scene = importer->ReadFile(
            path.string(),
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals);
        std::string stem_string = path.stem().string();
        //ASSERT(ai_scene != nullptr, "Failed to load scene!");
        std::cout<<path<<std::endl;
        std::cout<<(ai_scene != nullptr)<<std::endl;
        for (uint32_t mesh_i = 0; mesh_i < ai_scene->mNumMeshes; mesh_i++)
        {
            const aiMesh *ai_mesh = ai_scene->mMeshes[mesh_i];
            _meshes.emplace_back();

            TMesh &msh = _meshes.back();
            msh._vertices.resize(ai_mesh->mNumVertices, 3);
            msh._tex_coords.resize(ai_mesh->mNumVertices, 2);
            msh._normals.resize(ai_mesh->mNumVertices, 3);
            for (uint32_t vi = 0; vi < ai_mesh->mNumVertices; vi++)
            {
                const aiVector3D &vert = ai_mesh->mVertices[vi];
                const aiVector3D &norm = ai_mesh->mNormals[vi];
                const aiVector3D &tex = ai_mesh->mTextureCoords[0][vi];
                const aiVector3D &tan = ai_mesh->mTangents[vi];
                const aiVector3D &bitan = ai_mesh->mBitangents[vi];

                msh._vertices.row(vi) = Eigen::RowVector3d{vert.x, vert.y, vert.z};
                msh._tex_coords.row(vi) = Eigen::RowVector2d{tex.x, tex.y};
                msh._normals.row(vi) = Eigen::RowVector3d{norm.x, norm.y, norm.z};
            }
            msh._faces.resize(ai_mesh->mNumFaces, 3);
            for (uint32_t fi = 0; fi < ai_mesh->mNumFaces; fi++)
            {
                const aiFace &face = ai_mesh->mFaces[fi];
                msh._faces.row(fi) = Eigen::RowVector3i{static_cast<int>(face.mIndices[0]), static_cast<int>(face.mIndices[1]), static_cast<int>(face.mIndices[2])};
            }
        }
        delete importer;
    }

    void TMeshManager::addTexture(std::filesystem::path texture_path)
    {
        _textures.emplace_back(texture_path);
    }

}
