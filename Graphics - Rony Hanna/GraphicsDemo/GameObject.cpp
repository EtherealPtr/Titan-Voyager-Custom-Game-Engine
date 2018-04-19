#include "GameObject.h"
#include <string>

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	glDeleteBuffers(TOTAL_BUFFERS, &m_vao);
	glDeleteVertexArrays(1, &m_vao);
}

void GameObject::SetTextureId(char* texId)
{
	m_texId = texId;
}

void GameObject::SetObjectId(int id)
{
	m_objId = id;
}

char* GameObject::GetTextureId() const
{
	return m_texId;
}

int GameObject::GetObjectId() const
{
	return m_objId;
}

void GameObject::CreateGameObj(std::vector<Vertex> verts, GLuint numOfVertices, unsigned int* indices, GLuint numOfIndices, bool bInstancing)
{
	m_numOfVertices = numOfVertices;
	m_numOfIndices = numOfIndices;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	std::vector<glm::vec3> vertexPos;
	std::vector<glm::vec3> vertexCol;
	std::vector<glm::vec2> vertexUv;
	std::vector<glm::vec3> vertexNorms;

	vertexPos.reserve(m_numOfVertices);
	vertexUv.reserve(m_numOfVertices);
	vertexCol.reserve(m_numOfVertices);
	vertexNorms.reserve(m_numOfVertices);

	for (size_t i = 0; i < numOfVertices; ++i)
	{
		vertexPos.push_back(verts[i].GetPos());
		vertexCol.push_back(verts[i].GetCol());
		vertexUv.push_back(verts[i].GetTex());
		vertexNorms.push_back(verts[i].GetNormals());
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

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMALS_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexNorms[0]), &vertexNorms[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[ELEMENT_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	if (bInstancing)
	{
		// Do some instancing stuff for the basic shapes here in the future
	}

	glBindVertexArray(0);
}

void GameObject::Draw(Camera& camera)
{
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	this->shaderComponent.ActivateProgram();
	this->m_textureComponent.ActivateTexture();
	this->shaderComponent.SetMat4("projection", camera.GetProjectionMatrix());
	this->shaderComponent.SetMat4("view", camera.GetViewMatrix());
	this->shaderComponent.SetMat4("model", m_transform.GetModel());
	this->shaderComponent.SetVec3("lightPos", lightPos);
	this->shaderComponent.SetVec3("viewPos", camera.GetCameraPos());

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numOfIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	this->shaderComponent.DeactivateProgram();
}

void GameObject::DrawInstanced(glm::mat4 proj, glm::mat4 view)
{
	this->shaderComponent.ActivateProgram();
	this->m_textureComponent.ActivateTexture();
	this->shaderComponent.SetMat4("projection", proj);
	this->shaderComponent.SetMat4("view", view);
	this->shaderComponent.SetMat4("model", m_transform.GetModel());

	for (unsigned int i = 0; i < 1000; ++i)
	{
		glDrawElementsInstanced(GL_TRIANGLES, m_numOfIndices, GL_UNSIGNED_INT, nullptr, 30000);
	}

	glBindVertexArray(0);

	this->shaderComponent.DeactivateProgram();
}

void GameObject::SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_transform.SetPos(pos);
	m_transform.SetScale(scale);
	m_transform.SetRot(rot);
}