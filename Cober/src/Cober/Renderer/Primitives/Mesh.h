#pragma once

#include "Cober/Renderer/Shader.h"
#include "Cober/Renderer/Material.h"
#include "Cober/Renderer/Texture.h"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#define INVALID_MATERIAL 0xFFFFFFFF

namespace Cober {

    class Mesh {
    public:
        Mesh() {};
        ~Mesh();

        bool LoadMesh(const std::string& path);
        void Render();
        void Render(unsigned int NumInstances, const glm::vec4* MVPMats, const glm::vec4* WorldMats);
        // WorldTrans& GetWorldTransform() {return m_worldTransform; }
    private:
        void Clear();
        bool InitFromScene(const aiScene* pScene, const std::string& path);
        void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);
        void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);
        void InitAllMeshes(const aiScene* pScene);
        void InitSingleMesh(const aiMesh* paiMesh);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void PopulateBuffers();

    private:
        #define INVALID_MATERIAL 0xFFFFFFFF

        enum BUFFER_TYPE {
            POS_VB = 0,
            NORMAL_VB = 3,
            TEXCOORD_VB = 2,
            INDEX_BUFFER = 1,
            WVP_MAT_VB = 4,    // required only for instancing
            WORLD_MAT_VB = 5,  // required only for instancing
            NUM_BUFFERS = 6
        };
        //WorldTrans m_worldTransform;
        uint32_t m_VAO = 0;
        uint32_t m_Buffers[NUM_BUFFERS] = { 0 };

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
        //std::vector<Material> m_Materials;
        std::vector<Ref<Texture2D>> m_Textures;

        // Temporary space for vertex stuff before we load them into the GPU
        std::vector<glm::vec3> m_Positions;
        std::vector<glm::vec3> m_Normals;
        std::vector<glm::vec2> m_TexCoords;
        std::vector<unsigned int> m_Indices;
    };
}