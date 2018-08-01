#pragma once
#ifndef __FONT_H__
#define __FONT_H__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

#include <map>
#include <string>

struct Character
{
	GLuint m_textureID;
	glm::ivec2 m_size;
	glm::ivec2 m_bearing;
	GLuint m_advance;
};

class Text
{
public:
	Text() {};
	~Text() {};

	// Setters
	void SetPosition(glm::vec2 pos)	{ m_position = pos; }
	void SetColor(glm::vec3 col)	{ m_color = col; }
	void SetScale(GLfloat scale)	{ m_scale = scale; }
	void SetText(std::string text)	{ m_text = text; }
	void SetSpacing(GLfloat space)  { m_spacing = space; }

	void Configure(std::string font);
	void Render();

private:
	std::map<GLchar, Character> m_characters;
	GLuint m_vao, m_vbo;
	GLuint m_shaderProgram;
	std::string m_text;
	GLfloat m_scale, m_spacing;
	glm::vec3 m_color;
	glm::vec2 m_position;
};

#endif // !__FONT_H__