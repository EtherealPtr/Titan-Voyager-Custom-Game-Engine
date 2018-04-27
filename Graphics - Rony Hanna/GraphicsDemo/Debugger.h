#pragma once
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"
#include "Shader.h"
#include "Camera.h"

class Debugger
{
public:
	Debugger();
	~Debugger();

	void Init();
	void PrepareRayDebugger(glm::vec3& ray, glm::vec3& rayDir, Camera& cam);
	void DrawRay(Camera& cam);

private:
	Camera m_camera;
	GLuint m_vao;
	GLuint m_vbo;
	Shader m_shader;
	glm::mat4 model;
};

#endif // !__DEBUGGER_H__