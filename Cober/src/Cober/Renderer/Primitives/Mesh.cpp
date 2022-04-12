#include "pch.h"
#include "Mesh.h"

// PROVISIONAL
#include <Glew/glew.h>

namespace Cober {

#define POSITION_LOCATION  0
#define NORMAL_LOCATION    1
#define TEX_COORD_LOCATION 2

    Mesh::~Mesh()
    {
        Clear();
    }

    void Mesh::Clear()
    {
        if (m_Buffers[0] != 0)
            glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

        if (m_VAO != 0)
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
    }

    bool Mesh::LoadMesh(const std::string& Filename)
    {
        // Release the previously loaded mesh (if it exists)
        Clear();

        // Create the VAO
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        //glBindBuffer(GL_VERTEX_ARRAY, m_Buffers);

        // Create the buffers for the vertices attributes
        glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

        bool Ret = false;
        Assimp::Importer Importer;

        const aiScene* pScene = Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

        if (pScene)
            Ret = InitFromScene(pScene, Filename);
        else
            printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);

        return Ret;
    }

    bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
    {
        m_Meshes.resize(pScene->mNumMeshes);
        m_Materials.resize(pScene->mNumMaterials);

        unsigned int NumVertices = 0;
        unsigned int NumIndices = 0;

        CountVerticesAndIndices(pScene, NumVertices, NumIndices);

        ReserveSpace(NumVertices, NumIndices);

        InitAllMeshes(pScene);

        if (!InitMaterials(pScene, Filename))
            return false;

        PopulateBuffers();

        return GLCheckError();
    }


    void Mesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices)
    {
        for (unsigned int i = 0; i < m_Meshes.size(); i++) {
            m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
            m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
            m_Meshes[i].BaseVertex = NumVertices;
            m_Meshes[i].BaseIndex = NumIndices;

            NumVertices += pScene->mMeshes[i]->mNumVertices;
            NumIndices += m_Meshes[i].NumIndices;
        }
    }


    void Mesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
    {
        m_Positions.reserve(NumVertices);
        m_Normals.reserve(NumVertices);
        m_TexCoords.reserve(NumVertices);
        m_Indices.reserve(NumIndices);
    }


    void Mesh::InitAllMeshes(const aiScene* pScene)
    {
        for (unsigned int i = 0; i < m_Meshes.size(); i++) {
            const aiMesh* paiMesh = pScene->mMeshes[i];
            InitSingleMesh(paiMesh);
        }
    }


    void Mesh::InitSingleMesh(const aiMesh* paiMesh)
    {
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        // Populate the vertex attribute vectors
        for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {

            const aiVector3D& pPos = paiMesh->mVertices[i];
            m_Positions.push_back(glm::vec3(pPos.x, pPos.y, pPos.z));

            if (paiMesh->mNormals) {
                const aiVector3D& pNormal = paiMesh->mNormals[i];
                m_Normals.push_back(glm::vec3(pNormal.x, pNormal.y, pNormal.z));
            }
            else {
                aiVector3D Normal(0.0f, 1.0f, 0.0f);
                m_Normals.push_back(glm::vec3(Normal.x, Normal.y, Normal.z));
            }

            const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;
            m_TexCoords.push_back(glm::vec2(pTexCoord.x, pTexCoord.y));
        }

        // Populate the index buffer
        for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
            const aiFace& Face = paiMesh->mFaces[i];
            //        printf("num indices %d\n", Face.mNumIndices);
            //        assert(Face.mNumIndices == 3);
            m_Indices.push_back(Face.mIndices[0]);
            m_Indices.push_back(Face.mIndices[1]);
            m_Indices.push_back(Face.mIndices[2]);
        }
    }


    std::string GetDirFromFilename(const std::string& Filename)
    {
        // Extract the directory part from the file name
        std::string::size_type SlashIndex;

        SlashIndex = Filename.find_last_of("/");

        std::string Dir;

        if (SlashIndex == std::string::npos)
            Dir = ".";
        else if (SlashIndex == 0)
            Dir = "/";
        else
            Dir = Filename.substr(0, SlashIndex);

        return Dir;
    }


    bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
    {
        std::string Dir = GetDirFromFilename(Filename);

        bool Ret = true;

        // Initialize the materials
        for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
            const aiMaterial* pMaterial = pScene->mMaterials[i];

            LoadTextures(Dir, pMaterial, i);
            LoadColors(pMaterial, i);
        }

        return Ret;
    }


    void Mesh::LoadTextures(const std::string& Dir, const aiMaterial* pMaterial, int index)
    {
        LoadTexturesOfType(Dir, aiTextureType_DIFFUSE,      pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_SPECULAR,     pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_AMBIENT,      pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_EMISSIVE,     pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_HEIGHT,       pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_NORMALS,      pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_SHININESS,    pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_OPACITY,      pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_DISPLACEMENT, pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_LIGHTMAP,     pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_REFLECTION,   pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_BASE_COLOR,   pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_NORMAL_CAMERA,pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_METALNESS,    pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_DIFFUSE_ROUGHNESS, pMaterial, index);
        LoadTexturesOfType(Dir, aiTextureType_AMBIENT_OCCLUSION, pMaterial, index);
    }


    void Mesh::LoadTexturesOfType(const std::string& Dir, aiTextureType texType, const aiMaterial* pMaterial, int index)
    {
        m_Materials[index].textures.push_back(NULL);

        if (pMaterial->GetTextureCount(texType) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(texType, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string p(Path.data);

                //for (int i = 0; i < p.length(); i++)
                //    if (p[i] == '\\')
                //        p[i] = '/';

                if (p.substr(0, 2) == ".\\")
                    p = p.substr(2, p.size() - 2);

                std::string FullPath = Dir + "/" + p;

                printf("Loaded texture '%s' at index %d\n" , FullPath.c_str(), index);
                int lastIndex = m_Materials[index].textures.size() - 1;
                m_Materials[index].textures[lastIndex] = (Texture2D::Create(FullPath.c_str()));
                
                if (!m_Materials[index].textures[lastIndex])
                    printf("Error loading diffuse texture '%s'\n", FullPath.c_str());
                else
                    printf("Loaded diffuse texture '%s' at index %d\n", FullPath.c_str(), index);
            }
        }
    }
        
    void Mesh::LoadColors(const aiMaterial* pMaterial, int index)
    {
        aiColor3D AmbientColor(0.0f, 0.0f, 0.0f);
        if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, AmbientColor) == AI_SUCCESS) {
            printf("Loaded ambient color [%f %f %f]\n", AmbientColor.r, AmbientColor.g, AmbientColor.b);
            m_Materials[index].AmbientColor.r = AmbientColor.r;
            m_Materials[index].AmbientColor.g = AmbientColor.g;
            m_Materials[index].AmbientColor.b = AmbientColor.b;
        }

        aiColor3D DiffuseColor(0.0f, 0.0f, 0.0f);
        if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor) == AI_SUCCESS) {
            printf("Loaded diffuse color [%f %f %f]\n", DiffuseColor.r, DiffuseColor.g, DiffuseColor.b);
            m_Materials[index].DiffuseColor.r = DiffuseColor.r;
            m_Materials[index].DiffuseColor.g = DiffuseColor.g;
            m_Materials[index].DiffuseColor.b = DiffuseColor.b;
        }

        aiColor3D SpecularColor(0.0f, 0.0f, 0.0f);
        if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor) == AI_SUCCESS) {
            printf("Loaded specular color [%f %f %f]\n", SpecularColor.r, SpecularColor.g, SpecularColor.b);
            m_Materials[index].SpecularColor.r = SpecularColor.r;
            m_Materials[index].SpecularColor.g = SpecularColor.g;
            m_Materials[index].SpecularColor.b = SpecularColor.b;
        }
    }


    void Mesh::PopulateBuffers()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);
    }

    void Mesh::Render()
    {
        glBindVertexArray(m_VAO);

        for (unsigned int i = 0; i < m_Meshes.size(); i++) {
            unsigned int MaterialIndex = m_Meshes[i].MaterialIndex;
            assert(MaterialIndex < m_Materials.size());

            for (int type = 0; type < m_Materials[MaterialIndex].textures.size(); type++) {
                if (m_Materials[MaterialIndex].textures[type] != nullptr)
                    m_Materials[MaterialIndex].textures[type]->Bind(type);
            }

            glDrawElementsBaseVertex(GL_TRIANGLES,
                                     m_Meshes[i].NumIndices,
                                     GL_UNSIGNED_INT,
                                     (void*)(sizeof(unsigned int) * m_Meshes[i].BaseIndex),
                                     m_Meshes[i].BaseVertex);
        }

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);
    }

    const Material& Mesh::GetMaterial()
    {
        for (unsigned int i = 0; i < m_Materials.size(); i++)
            if (m_Materials[i].AmbientColor != glm::vec3(0.0f, 0.0f, 0.0f))
                return m_Materials[i];

        return m_Materials[0];
    }
}