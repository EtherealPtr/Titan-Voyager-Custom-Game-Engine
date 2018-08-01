#pragma once
#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include "Dependencies\glm-0.9.9-a2\glm\glm.hpp"
#include "Dependencies\glm-0.9.9-a2\glm\gtx\transform.hpp"

class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{}

	glm::mat4 GetModel() const
	{
		glm::mat4 posMatrix = glm::translate(m_pos);
		glm::mat4 rotXMatrix = glm::rotate(glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotYMatrix = glm::rotate(glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZMatrix = glm::rotate(glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(m_scale);

		glm::mat4 rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;

		return (posMatrix * rotMatrix * scaleMatrix);
	}

	glm::vec3& GetPos() { return m_pos; };
	glm::vec3& GetRot() { return m_rot; };
	glm::vec3& GetScale() { return m_scale; };

	void SetPos(const glm::vec3 pos) { m_pos = pos; };
	void SetRot(const glm::vec3& rot) { m_rot = rot; };
	void SetScale(const glm::vec3& scale) { m_scale = scale; };

private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif // !__TRANSFORMATION_H__