#pragma once
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Dependencies\glm-0.9.9-a2\glm\glm.hpp"
#include "Dependencies\glm-0.9.9-a2\glm\gtx\transform.hpp"

class Light
{
public:
	Light();
	~Light();

	void SetPos(glm::vec3 _pos) { m_pos = _pos; }
	glm::vec3& GetPos() { return m_pos; }

private:
	glm::vec3 m_pos;
};

#endif // !__LIGHT_H__