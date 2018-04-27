#include "Debugger.h"
#include <vector>

Debugger::Debugger() :
	model(1.0f)
{
}

Debugger::~Debugger()
{
}

void Debugger::Init()
{
	m_shader.CreateProgram("res/Shaders/Debugger.vs", "res/Shaders/Debugger.fs");
}

void Debugger::PrepareRayDebugger(glm::vec3& rayPos, glm::vec3& rayDir, Camera& cam)
{
	float vertices[] = {
		rayPos.x, rayPos.y, rayPos.z,
		rayPos.x, rayPos.y + 0.1f, rayPos.z,
		(rayDir.x * 50.0f) + rayPos.x, (rayDir.y * 50.0f) + rayPos.y, (rayDir.z * 50.0f) + rayPos.z
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindVertexArray(0);
}

void Debugger::DrawRay(Camera& cam)
{
	m_shader.ActivateProgram();

	m_shader.SetMat4("view", cam.GetViewMatrix());
	m_shader.SetMat4("projection", cam.GetProjectionMatrix());
	m_shader.SetMat4("model", model);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	m_shader.DeactivateProgram();
}