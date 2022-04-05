#pragma once
#include "Mesh.h"

// PROVISIONAL
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Cober {

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

    class Model
    {
    public:
        // model data 
        std::vector<MeshTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh>    meshes;
        std::string directory;
        bool gammaCorrection;

        // constructor, expects a filepath to a 3D model.
        Model() {}
        Model(std::string const& path, bool gamma = false);

        static Model Create(std::string const& path, bool gamma = false);

        // draws the model, and thus all its meshes
        void Draw(Ref<Shader> shader, const glm::vec3& position, const glm::vec3& size);
    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void LoadModel(std::string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };
}