#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Dependencies\soil\include\SOIL.h"
#include "Dependencies\glew\include\GL\glew.h"
#include <vector>

class Texture
{
public:
	Texture();
	~Texture();

	void GenerateTexture(char* textureId);
	void GenerateMultipleTextures(std::vector<char*>& textureIds);
	void ActivateTexture(unsigned int unit = 0);
	void ActivateTextures(unsigned int unit = 0);

	void GenerateSkybox();

private:
	GLuint m_texture;
	GLuint m_textures[32];
	GLuint m_skyboxTex;
};

#endif // !__TEXTURE_H__