#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Model.h"
#include "Camera.h"
#include "Terrain.h"

class Enemy
{
public:
	Enemy(Camera& cam);
	~Enemy();

	void InitMesh();
	void Draw();
	void Update(Terrain& terrain, Camera& cam);
	void SetPos(glm::vec3& pos);
	glm::vec3& GetPos() { return m_pos; }

	void Seek(Camera& target, float dt);

private:
	Camera m_camera;
	Model m_model;
	glm::vec3 m_pos;
	glm::vec3 m_velocity;
	float m_maximumSpeed;
};

#endif // !__ENEMY_H__