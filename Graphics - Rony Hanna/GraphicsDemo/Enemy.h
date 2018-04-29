#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Model.h"
#include "Camera.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void SetPos(glm::vec3& pos) { m_pos = pos; }
	glm::vec3& GetPos() { return m_pos; }

	void Seek(Camera& target, float dt);

private:
	glm::vec3 m_pos;
	glm::vec3 m_velocity;
	float m_maximumSpeed;
};

#endif // !__ENEMY_H__