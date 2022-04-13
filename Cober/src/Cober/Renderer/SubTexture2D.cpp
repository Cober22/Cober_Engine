#include "pch.h"
#include "SubTexture2D.h"

namespace Cober {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture) {

		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& spriteSize) {

		glm::vec2 min = { (coord.x * spriteSize.x) / texture->GetWidth(), (coord.y * spriteSize.y) / texture->GetHeight() };
		glm::vec2 max = { (coord.x + 1) * spriteSize.x / texture->GetWidth(), ((coord.y + 1) * spriteSize.y) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}