#pragma once

#define MAX_BONE_INFLUENCE 4
#include "Cober/Renderer/Shader.h"

// Provisional
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Glew/Glew.h>

#include "stb_image/stb_image.h"


namespace Cober {

    struct MeshVertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct MeshTexture {
        unsigned int id;
        std::string type;
        std::string path;
    };

	class Mesh {

    public:
        std::vector<MeshVertex>   vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshTexture>  textures;
        unsigned int VAO;

        Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);

        void Draw(Ref<Shader> shader, const glm::vec3& position, const glm::vec3& size);
    private:
        unsigned int VBO, EBO;
        void SetupMesh();
	};
}