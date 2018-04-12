#include "Mesh.h"
 
Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures, bool instancing)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	CreateMesh(instancing);
}

void Mesh::Draw(Camera camera, Shader shaderProgram, bool instancing)
{
	shaderProgram.ActivateProgram();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
	
		std::string number;
		std::string name = m_textures[i].m_type;
	
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
	
		glm::mat4 projMat = camera.GetProjectionMatrix();
		glm::mat4 viewMat = camera.GetViewMatrix();

		shaderProgram.SetMat4("projection", projMat);
		shaderProgram.SetMat4("view", viewMat);
		glUniform1i(glGetUniformLocation(shaderProgram.GetShaderProgram(), (name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	
	glBindVertexArray(m_vao);

	if (instancing)
	{
		glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, 25000);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);

	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::CreateMesh(bool instancing)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
 
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, m_Normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, m_TexCoords));

	if (instancing)
	{
		unsigned int amount = 25000;
		glm::mat4* m_modelMatricesIns;
		m_modelMatricesIns = new glm::mat4[amount];
		srand(static_cast<unsigned int>(time(NULL)));
		float radius = 250.0f;
		float offset = 25.0f;

		for (unsigned int i = 0; i < amount; ++i)
		{
			glm::mat4 model(1.0f);
			float _x, _y, _z;

			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 200.0f;  
			_x = sin(angle) * radius + displacement;

			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 35.0f;
			_y = displacement * 2.7f;

			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset - 495.0f;
			_z = cos(angle) * radius + displacement;

			model = glm::translate(model, glm::vec3(_x, _y, _z));

			float scale = (rand() % 20) / 100.0f + 0.1f;
			model = glm::scale(model, glm::vec3(scale));

			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			m_modelMatricesIns[i] = model;
		}

		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &m_modelMatricesIns[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < 1; ++i)
		{
			glBindVertexArray(m_vao);

			GLsizei mat4Size = sizeof(glm::mat4);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glEnableVertexAttribArray(6);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)0);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(sizeof(glm::vec4)));
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(2 * sizeof(glm::vec4)));
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	glBindVertexArray(0);
}
