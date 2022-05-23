#pragma once
#include "pch.h"
#include "stb_image/stb_image.h"
#include "Cober/Core/Core.h"

#include <glm/glm.hpp>

namespace Cober {

	class Texture
	{
	public:
		~Texture() {};
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	
	class Texture2D : public Texture
	{
	public:
		Texture2D(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{
			m_InternalFormat = GL_RGBA8, m_DataFormat = GL_RGBA;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			glGenerateMipmap(m_RendererID);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		};
		Texture2D(const std::string& path)
			: m_Path(path)
		{
			int width, height, channels;

			stbi_set_flip_vertically_on_load(1);
			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			if (!data) {
				const char* reason = "[unknown reason]";
				CB_ASSERT(stbi_failure_reason(), "Cannot load image %s: '%s'.\n", path.c_str(), stbi_failure_reason())
				//if (stbi_failure_reason())
				//	printf("Cannot load image %s: '%s'.\n", path.c_str(), stbi_failure_reason());
				std::exit(-1);
			}


			CB_ASSERT(data, "Failed to load image!");
			m_Width = width;
			m_Height = height;

			// [---------- BLENDING ----------]
			GLenum internalFormat = 0, dataFormat = 0;
			switch (channels) {
			case 4: internalFormat = GL_RGBA8;	dataFormat = GL_RGBA;
				break;
			case 3: internalFormat = GL_RGB8;	dataFormat = GL_RGB;
				break;
			}
			// If internalFormat and dataFormat are '0' is because format not supported
			CB_ASSERT(internalFormat && dataFormat, "Format not supported!");

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			// load and generate the texture
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		};
		~Texture2D() { glDeleteTextures(1, &m_RendererID); };

		virtual uint32_t GetWidth() const override	{ return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		void SetData(void* data, uint32_t size) override {
			uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
			CB_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Data must be entire texture!");
			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		};

		virtual void Bind(uint32_t slot = 0) const override { glBindTextureUnit(slot, m_RendererID); };

		static Ref<Texture2D> Create(uint32_t width, uint32_t height) { return CreateRef<Texture2D>(width, height); };
		static Ref<Texture2D> Create(const std::string& path) { return CreateRef<Texture2D>(path); };
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};


	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
			: m_Texture(texture)
		{
			m_TexCoords[0] = { min.x, min.y };
			m_TexCoords[1] = { max.x, min.y };
			m_TexCoords[2] = { max.x, max.y };
			m_TexCoords[3] = { min.x, max.y };
		};
		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& cellsize, const glm::vec2& spriteSize = { 1, 1 })
		{
			glm::vec2 min = { (coord.x * cellsize.x) / texture->GetWidth(), (coord.y * cellsize.y) / texture->GetHeight() };
			glm::vec2 max = { (coord.x + spriteSize.x) * cellsize.x / texture->GetWidth(), ((coord.y + spriteSize.y) * cellsize.y) / texture->GetHeight() };
			return CreateRef<SubTexture2D>(texture, min, max);
		};
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}

