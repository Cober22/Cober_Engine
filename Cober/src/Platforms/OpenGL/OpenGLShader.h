#pragma once

#include "Cober/Renderer/Shader.h"

namespace Cober {

	class OpenGLShader : public Shader {

	public:
		OpenGLShader(const char* vertexPath, const char* fragmentPath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetBool(const std::string& name, bool value) const override;
		virtual void SetInt(const std::string& name, bool value) const override;
		virtual void SetFloat(const std::string& name, bool value) const override;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
	private:
		uint32_t m_RendererID = 0;
	};
}
