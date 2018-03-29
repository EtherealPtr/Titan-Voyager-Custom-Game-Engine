#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Dependencies\soil\include\SOIL.h"
#include "Dependencies\glew\include\GL\glew.h"

class Texture
{
public:
	Texture();
	~Texture();

	void GenerateTexture(char* textureId);
	void ActivateTexture(unsigned int unit = 0);

private:
	GLuint m_texture;
};

#endif // !__TEXTURE_H__