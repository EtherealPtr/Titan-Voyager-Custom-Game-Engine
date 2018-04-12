#include "GameObject.h"
#include <sstream>
#include <string>
#include <cstdlib>>
#include <ctime>

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

	if (bInstancing)
	{
		unsigned int amount = 30000;
		m_modelMatricesIns = new glm::mat4[amount];
		srand(static_cast<unsigned int>(time(NULL)));
		float radius = 50.0f;
		float offset = 2.5f;

		for (unsigned int i = 0; i < amount; ++i)
		{
			glm::mat4 model;
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			float scale = (rand() % 20) / 100.0f + 0.05f;
			model = glm::scale(model, glm::vec3(x, y, z));

			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			m_modelMatricesIns[i] = model;
		}

		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &m_modelMatricesIns[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < amount; ++i)
		{
			glBindVertexArray(m_vao);

			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glEnableVertexAttribArray(6);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)4);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)8);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)12);

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	glBindVertexArray(0);
}

void GameObject::Draw(Camera cam)
{
	m_camera = cam;
	this->shaderComponent.ActivateProgram();
	this->m_textureComponent.ActivateTexture();
	this->shaderComponent.SetMat4("projection", m_camera.GetProjectionMatrix());
	this->shaderComponent.SetMat4("view", m_camera.GetViewMatrix());
	this->shaderComponent.SetMat4("model", m_transform.GetModel());

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