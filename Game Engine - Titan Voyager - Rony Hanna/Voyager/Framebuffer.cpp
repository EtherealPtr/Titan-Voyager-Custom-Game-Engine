#include "Framebuffer.h"
#include "Renderer.h"

Framebuffer::Framebuffer()
{}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteFramebuffers(1, &m_shadowFbo);
}

void Framebuffer::CreateFramebuffer()
{
	// Create and bind the new custom framebuffer
	glGenFramebuffers(1, &m_fbo);	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// Create new empty texture that will be rendered to then attach it to the framebuffer
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create render buffer to attach stencil and depth data to it 
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Renderer::GetInstance().GetWindowWidth(), Renderer::GetInstance().GetWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Ensure that the frame buffer is complete and ready to be used
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Unable to create custom framebuffer.");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::CreateShadowFramebuffer()
{
	// Create and bind the new custom framebuffer
	glGenFramebuffers(1, &m_shadowFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo);

	// Create new empty texture that will store depth data for shadow calculation
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Ensure that the frame buffer is complete and ready to be used
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Unable to create custom framebuffer.");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}