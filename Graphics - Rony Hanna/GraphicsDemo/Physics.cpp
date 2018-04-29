#include "Physics.h"
#include "Renderer.h"
#include <iostream>

Physics::Physics() :
	m_mouseX(0.0f),
	m_mouseY(0.0f),
	m_castRay(false)
{
}

Physics::~Physics()
{}

void Physics::Update(Camera& cam, float dt, std::vector<SDL_Event> events, Terrain& terrain)
{
	ProcessInput(cam, dt, events, terrain);

	if (m_castRay)
	{
		m_debugRayCastDraw = true;
		m_ray = CastRayFromWeapon(cam);
		CheckCollision(cam, terrain);
	}
}

void Physics::ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events, Terrain& terrain)
{
	for (auto i = events.begin(); i != events.end(); ++i)
	{
		switch (i->type)
		{
		case SDL_MOUSEMOTION:
		{
			m_mouseX = i->motion.x;
			m_mouseY = i->motion.y;
			cam.MouseUpdate(glm::vec2(i->motion.x, i->motion.y), dt);
			break;
		}
		
		case SDL_MOUSEBUTTONDOWN:
		{
			switch (i->button.button)
			{
			case SDL_BUTTON_LEFT:
				m_castRay = true;
				break;

			case SDL_BUTTON_RIGHT:			
				break;

			default: break;
			}
			break;
		}

		case SDL_MOUSEBUTTONUP:
		{
			switch (i->button.button)
			{
			case SDL_BUTTON_LEFT:
				m_castRay = false;
				break;

			default: break;
			}
			break;
		}
		}
	}
}

Ray Physics::CastRayFromMouse(Camera& cam)
{
	// screen space (viewport coordinates)
	float x = (2.0f * m_mouseX) / Renderer::GetInstance().GetWindowWidth() - 1.0f;
	float y = 1.0f - (2.0f * m_mouseY) / Renderer::GetInstance().GetWindowHeight();
	float z = 1.0f;

	// normalised device space
	glm::vec3 mouseNdcCoords = glm::vec3(x, y, z);
	glm::vec4 mouseClipCoords = glm::vec4(mouseNdcCoords.x, mouseNdcCoords.y, -1.0f, 1.0f);
	glm::mat4 invProjMat = glm::inverse(cam.GetProjectionMatrix());
	glm::vec4 eyeCoords = invProjMat * mouseClipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	glm::mat4 invViewMat = glm::inverse(cam.GetViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

	Ray ray;
	ray.pos = cam.GetCameraPos();
	ray.dir = rayDirection;

	return ray;
}

Ray Physics::CastRayFromWeapon(Camera & cam)
{
	Ray ray;
	ray.pos = cam.GetCameraPos();
	ray.dir = cam.GetCameraForward();
	return ray;
}

bool Physics::RaySphere(Camera& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z)
{
	// work out components of quadratic
	glm::vec3 v = glm::vec3(x, y, z) - cam.GetCameraPos();
	long double a = glm::dot(RayDirWorld, RayDirWorld);
	long double b = 2.0 * glm::dot(v, RayDirWorld);
	long double c = glm::dot(v, v) - SphereRadius * SphereRadius;
	long double b_squared_minus_4ac = b * b + (-4.0) * a * c;

	if (b_squared_minus_4ac == 0)
	{
		// One real root 
		return true;
	}
	else if (b_squared_minus_4ac > 0)
	{
		// Two real roots 
		long double x1 = (-b - sqrt(b_squared_minus_4ac)) / (2.0 * a);
		long double x2 = (-b + sqrt(b_squared_minus_4ac)) / (2.0 * a);

		if (x1 >= 0.0 || x2 >= 0.0)
			return true;
		if (x1 < 0.0 || x2 >= 0.0)
			return true;
	}

	// No real roots
	return false;
}

void Physics::CheckCollision(Camera& cam, Terrain& terrain)
{
	m_collision = RaySphere(cam, m_ray.dir, 3.0f, 50.0f, terrain.GetHeightOfTerrain(50.0f, 50.0f) + 25.0f, 50.0f);

	if (m_collision)
	{
		OnHit();
	}
}

bool Physics::OnHit()
{
	printf("Hello");
	return false;
}