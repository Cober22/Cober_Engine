#pragma once

#include "Cober/Renderer/Shader.h"
#include "Cober/Renderer/Material.h"
#include "Cober/Renderer/VertexArray.h"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#define INVALID_MATERIAL 0xFFFFFFFF

namespace Cober {

    class Mesh {

    public:
        Mesh() {};
        ~Mesh();

        bool LoadMesh(const std::string& Filename);
        void Render();
        const Material& GetMaterial();
        const uint32_t GetVAO() { return m_VAO; };
        const Ref<Shader> GetShader() { return shader; }
        void SetShader(Ref<Shader> customShader) { shader = customShader; }

    private:
        void Clear();
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);
        void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

        void InitAllMeshes(const aiScene* pScene);
        void InitSingleMesh(const aiMesh* paiMesh);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);

        void PopulateBuffers();

        void LoadTextures(const std::string& Dir, const aiMaterial* pMaterial, int index);
        void LoadTexturesOfType(const std::string& Dir, aiTextureType texType, const aiMaterial* pMaterial, int index);
        void LoadColors(const aiMaterial* pMaterial, int index);
    private:
        enum BUFFER_TYPE {
            INDEX_BUFFER = 0,
            POS_VB = 1,
            TEXCOORD_VB = 2,
            NORMAL_VB = 3,
            WVP_MAT_VB = 4,  // required only for instancing
            WORLD_MAT_VB = 5,  // required only for instancing
            NUM_BUFFERS = 6
        };

        //WorldTrans m_worldTransform;
        uint32_t m_VAO = 0;
        uint32_t m_Buffers[NUM_BUFFERS];
        Ref<Shader> shader;

        struct BasicMeshEntry {
            BasicMeshEntry()
            {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
            }

            unsigned int NumIndices;
            unsigned int BaseVertex;
            unsigned int BaseIndex;
            unsigned int MaterialIndex;
        };

        std::vector<BasicMeshEntry> m_Meshes;
        std::vector<Material> m_Materials;

        // Temporary space for vertex stuff before we load them into the GPU
        std::vector<glm::vec3> m_Positions;
        std::vector<glm::vec3> m_Normals;
        std::vector<glm::vec2> m_TexCoords;
        std::vector<unsigned int> m_Indices;
    };
}