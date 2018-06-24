#include "Font.h"

void Text::Configure(std::string font)
{
	FT_Library ft;
	FT_Face face;

	// Initialize freetype 
	if (FT_Init_FreeType(&ft))
		printf("ERROR: Failed to initialize FreeType Library.\n");

	if (FT_New_Face(ft, font.c_str(), 0, &face))
		printf("ERROR: Failed to load font.\n");

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Set text properties 
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_scale = 1.0f;
	m_spacing = 1.0f;
	SetPosition(m_position);

	// Craete text shader
	Shader shaderText;
	m_shaderProgram = shaderText.CreateProgram("res/Shaders/Font/Text.vs", "res/Shaders/Font/Text.fs");

	// Orthographic projection 
	glm::mat4 projection = glm::ortho(0.0f, 1440.0f, 0.0f, 900.0f);

	glUseProgram(m_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Loop through all 128 ASCII characters and retrieve their character glyphs
	for (GLubyte i = 0; i < 128; ++i)
	{
		// Load each character glyph
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			printf("ERROR: Failed to load glyph.\n");
			continue;
		}

		// Generate and configure texture parameters for each character glyph
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
					 GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store each character
		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		m_characters.insert(std::pair<GLchar, Character>(i, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType 
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void Text::Render()
{
	glm::vec2 textPos = m_position;

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable text shader program, texture, and bind vao
	glUseProgram(m_shaderProgram);
	glUniform3f(glGetUniformLocation(m_shaderProgram, "textColor"), m_color.x, m_color.y, m_color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vao);

	// Loop through all characters
	for (auto i = m_text.begin(); i != m_text.end(); ++i)
	{
		Character c = m_characters[*i];

		// Calculate origin position of the quad, size of quad, and generate 6 vertices to form 2D quad (glDrawArrays)
		GLfloat posX = textPos.x + c.m_bearing.x * m_scale;
		GLfloat posY = textPos.y - (c.m_size.y - c.m_bearing.y) * m_scale;
		GLfloat width = c.m_size.x * m_scale;
		GLfloat height = c.m_size.y * m_scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = 
		{
			{ posX, posY + height, 0.0f, 0.0f },
			{ posX, posY, 0.0f, 1.0f },
			{ posX + width, posY, 1.0f, 1.0f },

			{ posX, posY + height, 0.0f, 0.0f },
			{ posX + width, posY, 1.0f, 1.0f },
			{ posX + width, posY + height, 1.0f, 0.0f }
		};

		// Render glyph texture on the quad
		glBindTexture(GL_TEXTURE_2D, c.m_textureID);

		// Update content of VBO memory and draw quad
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance for the character glyph
		textPos.x += (c.m_advance >> 6) * m_spacing;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}