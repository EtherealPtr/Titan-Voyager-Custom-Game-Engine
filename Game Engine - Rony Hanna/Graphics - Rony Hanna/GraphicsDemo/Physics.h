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
	Physics();
	~Physics();

	void ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events);
	void Update(Camera& cam, float dt, std::vector<SDL_Event> events, std::vector<Enemy*> enemies);

	Ray CastRayFromMouse(Camera& cam);
	Ray CastRayFromWeapon(Camera& cam);

	bool RaySphere(Camera& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z);
	Ray& GetRay() { return m_ray; }

	bool GetDebugRayCastDraw() { return m_debugRayCastDraw; }

	void CheckCollision(Camera& cam, std::vector<Enemy*> enemies);
	bool OnHit();

private:
	Ray m_ray;
	float m_mouseX;
	float m_mouseY;
	bool m_debugRayCastDraw;
	bool m_collision;
	bool m_castRay;
};

#endif // !__PHYSICS_H__