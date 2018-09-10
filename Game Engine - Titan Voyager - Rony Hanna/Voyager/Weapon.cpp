#include "Weapon.h"
#include "Utils.h"
#include "Renderer.h"
#include "Physics.h"
#include "Audio.h"

Weapon::Weapon() :
	m_maxAmmo(0),
	m_ammoCount(0),
	m_currFireRateTime(0.0f),
	m_fireRate(0.0f),
	m_currReloadTime(0.0f),
	m_maxReloadTimer(0.0f)
{}

Weapon::~Weapon()
{}

void Weapon::Init(GLchar* path, Camera& camera, char* vs, char* fs)
{
	m_model.Init(path, camera, vs, fs, false);
}

void Weapon::Configure(int maxAmmo, float fireRate, float reloadTime, int damage)
{
	m_maxAmmo = maxAmmo;
	m_fireRate = fireRate;
	m_maxReloadTimer = reloadTime;
	m_currFireRateTime = m_fireRate;
	m_ammoCount = m_maxAmmo;
	m_damage = damage;
}

void Weapon::RestartWeapon()
{
	m_ammoCount = m_maxAmmo;
}

void Weapon::Update(Camera& cam, float dt)
{
	if (m_currFireRateTime > m_fireRate)
	{
		m_currFireRateTime = m_fireRate;
	}
	else
	{
		m_currFireRateTime += 0.79f * dt;
	}
}

void Weapon::Fire(Model& weapon, Camera& cam, float dt, bool& firing, bool& reloading)
{
	m_animator.PlayIdleFPS(weapon, cam, dt);

	if (m_currFireRateTime > m_fireRate)
	{
		Physics::GetInstance().CastRay();
		m_animator.PlayFireFPS(weapon, cam, dt);
		Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("AR_Fire")->second);

		--m_ammoCount;

		if (m_ammoCount <= 0)
		{
			m_ammoCount = 0;
			firing = false;
			reloading = true;
		}

		// Play muzzle flash effect (render textured quad in eye space in front of weapon)
		glm::mat4 model(1.0f);
		glm::mat4 translation = glm::translate(glm::vec3(0.9f, -1.4f, -6.5f));
		glm::mat4 rotation = glm::rotate(Utils::GetInstance().RandomNumBetweenTwo(1.0f, 360.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMat = glm::scale(glm::vec3(Utils::GetInstance().RandomNumBetweenTwo(2.0f, 2.5f), Utils::GetInstance().RandomNumBetweenTwo(2.0f, 2.5f), 1.0f));
		glm::mat4 invViewMat = glm::inverse(cam.GetViewMatrix());
		model = invViewMat * translation * rotation * scaleMat;
		Renderer::GetInstance().GetComponent(8).Draw(model, cam, glm::vec3(0.0f, 0.0f, 0.0f));
		m_currFireRateTime = 0.0f;
	}
}

void Weapon::Reload(Model& weapon, Camera& cam, float dt, bool& reloading)
{
	m_animator.PlayReloadFPS(weapon, cam, dt);
	m_currReloadTime += 0.4f * dt;

	if (m_currReloadTime >= m_maxReloadTimer)
	{
		m_ammoCount = m_maxAmmo;
		m_currReloadTime = 0.0f;
		reloading = false;
	}
}