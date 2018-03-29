#include "Texture.h"
#include <cassert>
#include <iostream>
#include "ResourceManager.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

// -------------------
// Author: Rony Hanna
// Description: Function that specify a 2D texture image 
// -------------------
void Texture::GenerateTexture(char* textureId)
{
	unsigned char* image = ResourceManager::GetInstance().GetTexture(textureId);
	int w = ResourceManager::GetInstance().GetImageWidth(textureId);
	int h = ResourceManager::GetInstance().GetImageHeight(textureId);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

// -------------------
// Author: Rony Hanna
// Description: Function that activates (or binds) a texture 
// -------------------
void Texture::ActivateTexture(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}