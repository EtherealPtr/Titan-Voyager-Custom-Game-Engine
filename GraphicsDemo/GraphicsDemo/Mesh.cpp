#include "Mesh.h"
#include <vector>
#include <iostream>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	glDeleteBuffers(TOTAL_BUFFERS, &m_vao);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::SetTextureId(char* texId)
{
	m_texId = texId;
}

char* Mesh::GetTextureId() const
{
	return m_texId;
}

void Mesh::CreateMesh(Vertex* verts, GLuint numOfVertices, unsigned int* indices, GLuint numOfIndices)
{
	m_numOfVertices = numOfVertices;
	m_numOfIndices = numOfIndices;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	std::vector<glm::vec3> vertexPos;
	std::vector<glm::vec3> vertexCol;
	std::vector<glm::vec2> vertexUv;

	vertexPos.reserve(m_numOfVertices);
	vertexUv.reserve(m_numOfVertices);
	vertexCol.reserve(m_numOfVertices);

	for (size_t i = 0; i < numOfVertices; ++i)
	{
		vertexPos.push_back(verts[i].GetPos());
		vertexCol.push_back(verts[i].GetCol());
		vertexUv.push_back(verts[i].GetTex());
	}

	glGenBuffers(TOTAL_BUFFERS, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[POSITION_VB]); 
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexPos[0]), &vertexPos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexCol[0]), &vertexCol[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[UV_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexUv[0]), &vertexUv[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[ELEMENT_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	model = glm::translate(glm::vec3(0.0f, 0.3f, -1.0f));
	model = glm::rotate(model, (float)SDL_GetTicks() * 0.001f, glm::vec3(1.0f, 0.0f, 1.0f));

	//m_camera.InitCameraOrthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 600.0f);
	m_camera.InitCameraPerspective(80.0f, 800.0f / 600.0f, 1.0f, 500.0f);
	m_camera.SetCameraPos(glm::vec3(0.0f, 0.0f, 1.0f));
	m_camera.Update();
	this->shaderComponent.SetMat4("projection", m_camera.GetProjectionMatrix());
	this->shaderComponent.SetMat4("view", m_camera.GetViewMatrix());
	this->shaderComponent.SetMat4("model", model);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numOfIndices, GL_UNSIGNED_INT, nullptr); 
	glBindVertexArray(0);

	this->shaderComponent.DeactivateProgram();
}