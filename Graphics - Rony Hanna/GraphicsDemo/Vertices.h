#pragma once
#ifndef __VERTICES_H__
#define __VERTICES_H__

#include "Dependencies/glm-0.9.9-a2/glm/glm.hpp"

class Vertex
{
public:
	Vertex() {};
	Vertex(glm::vec3& pos, glm::vec3& col, glm::vec2& tex, glm::vec3& normals, glm::vec3& tangent = glm::vec3(1.0f)) 
	{
		this->m_pos = pos;
		this->m_col = col;
		this->m_tex = tex;
		this->m_normals = normals;
		this->m_tangent = tangent;
	}

	glm::vec3& GetPos() { return m_pos; }
	glm::vec3& GetCol() { return m_col; }
	glm::vec2& GetTex() { return m_tex; }
	glm::vec3& GetNormals() { return m_normals; }
	glm::vec3& GetTangent() { return m_tangent; }

private:
	glm::vec3 m_pos;
	glm::vec3 m_col;
	glm::vec2 m_tex;
	glm::vec3 m_normals;
	glm::vec3 m_tangent;
};

#endif // !__VERTICES_H__