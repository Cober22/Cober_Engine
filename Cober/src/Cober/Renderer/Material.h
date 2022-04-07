#pragma once

#include "Cober/Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Cober {
    class Material {

    public:
        glm::vec3 AmbientColor  = glm::vec3(0.0f);
        glm::vec3 DiffuseColor  = glm::vec3(0.0f);
        glm::vec3 SpecularColor = glm::vec3(0.0f);

        

        Ref<Texture2D> pSpecularExponent = NULL; // base color of the material
        // TODO: need to deallocate these
        // 
        std::vector<Ref<Texture2D>> textures; //Size
        // 
        //std::vector<Ref<Texture2D>> MaterialTextures[sizeof(matTextureTypes)];
    };
}