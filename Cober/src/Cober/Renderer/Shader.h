#pragma once

#include <glm/glm.hpp>

namespace Cober {

	class Shader {

	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, bool value) const = 0;
		virtual void SetInt(const std::string& name, bool value) const = 0;
		virtual void SetFloat(const std::string& name, bool value) const = 0;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;

		static Shader* Create(const char* vertexPath, const char* fragmentPath);
	};
}




