#pragma once

#include "pch.h"
#include <GL/GL.h>
#include "Cober/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Cober {

	class RenderCommand {

	public:

		static void Init() 
		{
			glEnable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_MULTISAMPLE);
			//glEnable(GL_ALPHA_TEST);
			//glAlphaFunc(GL_GREATER, 0.5F);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBlendFunc(GL_ONE, GL_ZERO);
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { glViewport(x, y, width, height);	}

		static void SetClearColor(const glm::vec4& color) {	glClearColor(color.r, color.g, color.b, color.a);}

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

}