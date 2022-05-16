#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Cober {

	class Shader {

	public:
		Shader(const std::string& filePath);
		Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader() { glDeleteProgram(m_RendererID); };

		void Bind() const { glUseProgram(m_RendererID); };
		void Unbind() const { glUseProgram(0); };

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetVec2(const std::string& name, const glm::vec2& value);
		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetVec4(const std::string& name, const glm::vec4& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		const std::string& GetName() const;
		const uint32_t GetID() const;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) { return CreateRef<Shader>(name, vertexSrc, fragmentSrc); };
		static Ref<Shader> Create(const std::string& filePath) { return CreateRef<Shader>(filePath); };

		void UploadUniformBool(const std::string& name, bool value);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformVec2(const std::string& name, const glm::vec2& value);
		void UploadUniformVec3(const std::string& name, const glm::vec3& value);
		void UploadUniformVec4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
	};


	class ShaderLibrary 
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Name(const std::string& name, const std::string& filePath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}