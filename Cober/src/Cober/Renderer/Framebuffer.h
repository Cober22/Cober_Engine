#pragma once

#include "Cober/Core/Core.h"

namespace Cober {

	enum class FramebufferTextureFormat
	{
		None = 0,
		// Color
		RGBA8,
		RED_INTEGER,
		// Depth / Stencil
		DEPTH24STENCIL8,
		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};


	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};


	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {

		static GLenum TextureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}
		static void BindTexture(bool multisampled, uint32_t id) {
			glBindTexture(TextureTarget(multisampled), id);
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {

			bool multisampled = samples > 1;
			if (multisampled)
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {

			bool multisampled = samples > 1;
			if (multisampled)
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}
		static bool IsDepthFormat(FramebufferTextureFormat format) {

			switch (format) {
			case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}
		static GLenum CoberTextureFormatToGL(FramebufferTextureFormat format) {

			switch (format) {
			case FramebufferTextureFormat::RGBA8:		return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER:	return GL_RED_INTEGER;
			}
			CB_ASSERT(false);
			return 0;
		}
	}

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec) 
			: m_Specification(spec)
		{
			for (auto spec : m_Specification.Attachments.Attachments) {
				if (!Utils::IsDepthFormat(spec.TextureFormat))
					m_ColorAttachmentSpecification.emplace_back(spec);
				else
					m_DepthAttachmentSpecification = spec;
			}
			Invalidate();
		};

		virtual ~Framebuffer() {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachment.size(), m_ColorAttachment.data());
			glDeleteTextures(1, &m_DepthAttachment);
		};

		void Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
			glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		};

		void Unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		};

		void Resize(uint32_t width, uint32_t height) {
			m_Specification.Width = width;
			m_Specification.Height = height;
			Invalidate();
		};

		int ReadPixel(uint32_t attachmentIndex, int x, int y) {
			CB_ASSERT(attachmentIndex < m_ColorAttachment.size());
			glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
			int pixelData;
			glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
			return pixelData;
		};

		void ClearAttachment(uint32_t attachmentIndex, int value) {
			CB_ASSERT(attachmentIndex < m_ColorAttachment.size());
			auto& spec = m_ColorAttachmentSpecification[attachmentIndex];
			glClearTexImage(m_ColorAttachment[attachmentIndex], 0,
				Utils::CoberTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
		};

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const {
			CB_ASSERT(index < m_ColorAttachment.size()); return m_ColorAttachment[index];
		};

		const FramebufferSpecification& GetSpecification() const { return m_Specification; };

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec) {
			Ref<Framebuffer> FBO = CreateRef<Framebuffer>(spec);
			return FBO;
		};

		void Invalidate() {
			if (m_RendererID)
			{
				glDeleteFramebuffers(1, &m_RendererID);
				glDeleteTextures(m_ColorAttachment.size(), m_ColorAttachment.data());
				glDeleteTextures(1, &m_DepthAttachment);

				m_ColorAttachment.clear();
				m_DepthAttachment = 0;
			}

			glCreateFramebuffers(1, &m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

			bool multisample = m_Specification.Samples > 1;
			//Attachments
			if (m_ColorAttachmentSpecification.size()) {

				m_ColorAttachment.resize(m_ColorAttachmentSpecification.size());
				Utils::CreateTextures(multisample, m_ColorAttachment.data(), m_ColorAttachment.size());

				for (size_t i = 0; i < m_ColorAttachmentSpecification.size(); i++) {

					Utils::BindTexture(multisample, m_ColorAttachment[i]);
					switch (m_ColorAttachmentSpecification[i].TextureFormat)
					{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachment[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(m_ColorAttachment[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
						break;
					}
				}
			}

			if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {

				Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
				Utils::BindTexture(multisample, m_DepthAttachment);
				switch (m_DepthAttachmentSpecification.TextureFormat) {
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
				}
			}

			if (m_ColorAttachment.size() > 1)
			{
				CB_ASSERT(m_ColorAttachment.size() <= 4);
				GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
				glDrawBuffers(m_ColorAttachment.size(), buffers);
			}
			else if (m_ColorAttachment.empty()) {
				// Only depth-pass
				glDrawBuffer(GL_NONE);
			}

			CB_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		};
	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecification;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachment;
		uint32_t m_DepthAttachment = 0;
	};
}