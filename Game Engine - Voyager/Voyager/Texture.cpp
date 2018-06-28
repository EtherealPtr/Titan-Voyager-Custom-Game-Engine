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
	std::vector<int> imgDimension;
	unsigned char* image = ResourceManager::GetInstance().GetTexture(textureId);
	ResourceManager::GetInstance().GetImageDimension(textureId, imgDimension);

	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgDimension.at(0), imgDimension.at(1), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	imgDimension.clear();
}

// -------------------
// Author: Rony Hanna
// Description: Function that generates multiple textures and binds them to appropriate texture unit
// -------------------
void Texture::GenerateMultipleTextures(std::vector<char*>& textureIds)
{
	std::vector<unsigned char*> images(textureIds.size(), 0);
	std::vector<int> imgDimension;

	glGenTextures(textureIds.size(), m_textures);

	for (unsigned int i = 0; i < textureIds.size(); ++i)
	{
		images[i] = ResourceManager::GetInstance().GetTexture(textureIds.at(i));
		ResourceManager::GetInstance().GetImageDimension(textureIds.at(i), imgDimension);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		// Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Texture filtering
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgDimension.at(0), imgDimension.at(1), 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]);
		imgDimension.clear();
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
	std::vector<int> imgDimension;

	for (unsigned int i = beginIndex; i < finalIndex; ++i)
	{
		unsigned char* image = ResourceManager::GetInstance().GetTexture(ResourceManager::GetInstance().GetSkyboxTextureIds()[i]);
		ResourceManager::GetInstance().GetImageDimension(ResourceManager::GetInstance().GetSkyboxTextureIds()[i], imgDimension);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + c, 0, GL_RGBA, imgDimension.at(0), imgDimension.at(1), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		imgDimension.clear();
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