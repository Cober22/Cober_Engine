#pragma once

#include "Cober/Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Cober {
    class Material {

    public:
        glm::vec3 AmbientColor  = glm::vec3(0.0f);
        glm::vec3 DiffuseColor  = glm::vec3(0.0f);
        glm::vec3 SpecularColor = glm::vec3(0.0f);

        // TODO: need to deallocate these
        Ref<Texture2D> pDiffuse = NULL; // base color of the material
        Ref<Texture2D> pSpecularExponent = NULL;
    };
}