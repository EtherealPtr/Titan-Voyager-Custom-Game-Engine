#include "Animation.h"

const float WEAPON_ROTATION_190_DEG = 3.31613f;

Animation::Animation() :
	m_defWeaponRotation(WEAPON_ROTATION_190_DEG),
	m_defWeaponZOffset(-2.5f),
	m_defWeaponYOffset(-2.0f),
	weaponMoveForward(false),
	weaponMoveUp(true)
{}

Animation::~Animation()
{}

void Animation::PlaySprintFPS(Model& gun, Camera& cam, float dt)
{
	const float MAX_ROTATION = 4.5f;
	const float MAX_Y_OFFSET = -2.0f;
	const float MIN_Y_OFFSET = -2.5f;
	m_defWeaponRotation += 7.0f * dt;

	if (m_defWeaponRotation >= MAX_ROTATION)
		m_defWeaponRotation = MAX_ROTATION;

	if (weaponMoveUp)
	{
		m_defWeaponYOffset += 1.0f * dt;

		if (m_defWeaponYOffset >= MAX_Y_OFFSET)
		{
			weaponMoveUp = false;
			m_defWeaponYOffset = MAX_Y_OFFSET;
		}
	}
	else
	{
		m_defWeaponYOffset -= 1.0f * dt;

		if (m_defWeaponYOffset <= MIN_Y_OFFSET)
		{
			weaponMoveUp = true;
			m_defWeaponYOffset = MIN_Y_OFFSET;
		}
	}

	gun.Draw(cam, glm::vec3(1.7f, m_defWeaponYOffset, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlayWalkFPS(Model& gun, Camera& cam, float dt)
{
	const float MAX_Z_OFFSET = -2.9f;
	const float MIN_Z_OFFSET = -2.5f;
	m_defWeaponYOffset = -2.5f;

	// Rotate the weapon back to 190 degrees if it isn't at 190 degrees 
	if (m_defWeaponRotation >= WEAPON_ROTATION_190_DEG)
	{
		m_defWeaponRotation -= 7.0f * dt;

		if (m_defWeaponRotation <= WEAPON_ROTATION_190_DEG)
			m_defWeaponRotation = WEAPON_ROTATION_190_DEG;
	}

	if (m_defWeaponZOffset <= MAX_Z_OFFSET)
		weaponMoveForward = false;
	else if (m_defWeaponZOffset >= MIN_Z_OFFSET)
		weaponMoveForward = true;

	if (weaponMoveForward)
	{
		m_defWeaponZOffset -= 1.0f * dt;

		// Prevent weapon from going too far into the world
		if (m_defWeaponZOffset <= MAX_Z_OFFSET)
			m_defWeaponZOffset = MAX_Z_OFFSET;
	}
	else
	{
		m_defWeaponZOffset += 1.0f * dt;

		// Prevent weapon from going too far behind the view
		if (m_defWeaponZOffset >= MIN_Z_OFFSET)
			m_defWeaponZOffset = MIN_Z_OFFSET;
	}

	gun.Draw(cam, glm::vec3(1.7f, -2.0f, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlayIdleFPS(Model& gun, Camera& cam, float dt)
{
	if (m_defWeaponZOffset <= -2.5f)
	{
		m_defWeaponZOffset += 1.5f * dt;

		if (m_defWeaponZOffset >= -2.5f)
			m_defWeaponZOffset = -2.5f;
	}

	gun.Draw(cam, glm::vec3(1.7f, -2.0f, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), WEAPON_ROTATION_190_DEG, glm::vec3(1.0f, 1.0f, 1.0f), true);
}