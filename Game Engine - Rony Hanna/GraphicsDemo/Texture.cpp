#include "Texture.h"
#include <cassert>
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
	glActiveTexture(GL_TEXTURE0);
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
// Description: Function that generates multiple textures and binds them to appropriate texture unit
// -------------------
void Texture::GenerateMultipleTextures(std::vector<char*>& textureIds)
{
	std::vector<unsigned char*> images(textureIds.size(), 0);

	glGenTextures(textureIds.size(), m_textures);

	for (unsigned int i = 0; i < textureIds.size(); ++i)
	{
		images[i] = ResourceManager::GetInstance().GetTexture(textureIds.at(i));
		int w = ResourceManager::GetInstance().GetImageWidth(textureIds.at(i));
		int h = ResourceManager::GetInstance().GetImageHeight(textureIds.at(i));

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		// Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Texture filtering
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);
	}
}

// -------------------
// Author: Rony Hanna
// Description: Function that generates a cubemap texture
// -------------------
void Texture::GenerateSkybox(unsigned short int beginIndex, unsigned short int finalIndex)
{
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	int c = 0;

	for (unsigned int i = beginIndex; i < finalIndex; ++i)
	{
		unsigned char* image = ResourceManager::GetInstance().GetTexture(ResourceManager::GetInstance().GetSkyboxTextureIds()[i]);
		int w = ResourceManager::GetInstance().GetImageWidth(ResourceManager::GetInstance().GetSkyboxTextureIds()[i]);
		int h = ResourceManager::GetInstance().GetImageHeight(ResourceManager::GetInstance().GetSkyboxTextureIds()[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + c, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		c++;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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

// -------------------
// Author: Rony Hanna
// Description: Function that activates (or binds) multiple textures  
// -------------------
void Texture::ActivateTextures(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_textures[unit]);
}