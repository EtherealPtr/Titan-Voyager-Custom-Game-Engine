#pragma once
#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void CreateFramebuffer();
	void CreateShadowFramebuffer();
	void ActivateShadowFramebuffer()	{ glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFbo); }
	void ActivateFramebuffer()			{ glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
	void DeactivateFramebuffer()		{ glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	GLuint GetColorBufferTexture() { return m_texture; }
	GLuint GetDepthBufferTexture() { return m_depthTexture; }

private:
	GLuint m_fbo, m_rbo, m_shadowFbo;
	GLuint m_texture, m_depthTexture;
};

#endif // !__FRAMEBUFFER_H__