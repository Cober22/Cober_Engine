#pragma once

#include "Cober/Renderer/Framebuffer.h"

namespace Cober {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height);

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { CB_ASSERT(index < m_ColorAttachment.size()); return m_ColorAttachment[index]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecification;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachment;
		uint32_t m_DepthAttachment = 0;
	};
}