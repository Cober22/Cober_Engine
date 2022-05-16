#pragma once

#include <Bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace Cober {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
	bool DecomposeTransform(const btTransform& transform, glm::vec3& outTranslation, glm::vec3& outRotation);
}