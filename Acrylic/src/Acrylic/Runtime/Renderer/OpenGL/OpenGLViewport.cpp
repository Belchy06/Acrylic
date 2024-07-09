#include "acpch.h"
#include "OpenGLViewport.h"

#include <glad/glad.h>

namespace Acrylic
{
	OpenGLViewport::OpenGLViewport(uint32_t Width, uint32_t Height)
	{
		Resize(Width, Height);
	}

	OpenGLViewport::~OpenGLViewport()
	{
		glDeleteFramebuffers(1, &RendererId);
	}

	void OpenGLViewport::Resize(uint32_t Width, uint32_t Height)
	{
		// TODO: Parse command line for HDR
		CreateTextureDesc Desc = {};
		Desc.Width = Width;						//
		Desc.Height = Height;					//
		Desc.PixelFormat = EPixelFormat::RGBA8; //

		if (RendererId || Backbuffer)
		{
			glDeleteFramebuffers(1, &RendererId);
			Backbuffer = nullptr;
		}

		glCreateFramebuffers(1, &RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, RendererId);

		Backbuffer = Texture2D::Create(Desc);

		glBindTexture(GL_TEXTURE_2D, Backbuffer->GetID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Backbuffer->GetID(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLViewport::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RendererId);
		glViewport(0, 0, Backbuffer->GetDesc().Width, Backbuffer->GetDesc().Height);
	}

	void OpenGLViewport::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
} // namespace Acrylic