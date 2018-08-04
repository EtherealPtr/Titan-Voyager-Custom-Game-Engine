#pragma once
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Camera.h"
#include "Dependencies/SDL2/include/SDL.h"
#include <vector>
#include "Enemy.h"

struct Ray
{
	glm::vec3 pos;
	glm::vec3 dir;
};

class Physics
{
public:
	~Physics();

	static Physics& GetInstance()
	{
		static Physics instance;
		return instance;
	}

	Physics(Physics const&) = delete;

	void operator=(Physics const&) = delete;

	void ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events);
	void Update(Camera& cam, float dt, std::vector<SDL_Event> events, std::vector<Enemy*>& enemies);

	void CastRay()					{ m_castRay = true; }
	Ray& GetRay()					{ return m_ray; }
	bool GetDebugRayCastDraw()		{ return m_debugRayCastDraw; }
	float GetGravity()				{ return m_gravity; }
	void OnEnemyHit(Enemy* enemy);
	void OnPlayerHit(float damage);
	bool PointInSphere(Camera& cam, glm::vec3&, float radius);

private:
	Physics();

	Ray m_ray;
	float m_mouseX, m_mouseY;
	bool m_debugRayCastDraw;
	bool m_collision;
	bool m_castRay;
	float m_gravity;

	// Private functions
	Ray CastRayFromMouse(Camera& cam);
	Ray CastRayFromWeapon(Camera& cam);
	void CheckRaySphereCollision(Camera& cam, std::vector<Enemy*> enemies);
	bool RaySphere(Camera& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z);
};

#endif // !__PHYSICS_H__