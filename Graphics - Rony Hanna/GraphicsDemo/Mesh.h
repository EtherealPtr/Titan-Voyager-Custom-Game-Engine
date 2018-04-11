#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include "Dependencies\assimp\include\assimp\types.h"
#include "Dependencies\assimp\include\assimp\Importer.hpp"
#include "Dependencies\assimp\include\assimp\scene.h"
#include "Dependencies\assimp\include\assimp\postprocess.h"
#include "Dependencies\glm-0.9.9-a2\glm\glm.hpp"
#include "Dependencies\glm-0.9.9-a2\glm\gtx\transform.hpp"
#include <string>
#include <vector>
#include "Shader.h"
#include <sstream>
#include "Camera.h"

struct MeshVertex 
{
public:
	MeshVertex() {};
	~MeshVertex() {};

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MeshTexture 
{
public:
	MeshTexture() {};
	~MeshTexture() {};

	GLuint id;
	std::string type;
	aiString m_path;
};

class Mesh 
{
public:
	std::vector<MeshVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<MeshTexture> textures;

	Mesh(std::vector<MeshVertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->setupMesh();
	}

	void Draw(Camera* camera, GLuint program)
	{
		glUseProgram(program);

		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); 
											   
			std::stringstream ss;
			std::string number;
			std::string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; 
			else if (name == "texture_specular")
				ss << specularNr++;

			number = ss.str();
			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}

		glm::mat4 model(1.0f);
		glm::mat4 mvp = camera->GetProjectionMatrix() *  camera->GetViewMatrix() * model;

		GLint mvpLoc = glGetUniformLocation(program, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (GLuint i = 0; i < this->textures.size(); i++) 
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	GLuint VAO, VBO, EBO;

	void setupMesh()
	{
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(MeshVertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, TexCoords));

		glBindVertexArray(0);
	}
};

#endif // !__MESH_H__