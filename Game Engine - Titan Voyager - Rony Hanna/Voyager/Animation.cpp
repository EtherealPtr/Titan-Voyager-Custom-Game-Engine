#include "Animation.h"

const float WEAPON_ROTATION_190_DEG = 3.31613f;

Animation::Animation() :
	m_defWeaponRotation(WEAPON_ROTATION_190_DEG),
	m_defWeaponZOffset(-2.5f),
	m_originalWeaponZOffset(-2.5f),
	m_sprintWeaponZOffset(-2.5f),
	m_defWeaponYOffset(-2.0f),
	m_idleYOffset(-2.0f),
	m_weaponMoveForward(false),
	m_weaponMoveUp(true),
	m_swapping(false)
{}

Animation::~Animation()
{}

void Animation::SetWeaponZOffset(float zOffset)
{
	m_originalWeaponZOffset = zOffset;
	m_defWeaponZOffset = zOffset;
}

void Animation::PlaySprintFPS(Model& weapon, Camera& cam, float dt)
{
	const float MAX_ROTATION = 4.5f;
	const float MAX_Y_OFFSET = -2.0f;
	const float MIN_Y_OFFSET = -2.5f;

	m_defWeaponRotation += 7.0f * dt;

	if (m_defWeaponRotation >= MAX_ROTATION)
		m_defWeaponRotation = MAX_ROTATION;

	if (m_weaponMoveUp)
	{
		m_defWeaponYOffset += 1.0f * dt;

		if (m_defWeaponYOffset >= MAX_Y_OFFSET)
		{
			m_weaponMoveUp = false;
			m_defWeaponYOffset = MAX_Y_OFFSET;
		}
	}
	else
	{
		m_defWeaponYOffset -= 1.0f * dt;

		if (m_defWeaponYOffset <= MIN_Y_OFFSET)
		{
			m_weaponMoveUp = true;
			m_defWeaponYOffset = MIN_Y_OFFSET;
		}
	}

	weapon.Draw(cam, glm::vec3(1.7f, m_defWeaponYOffset, m_sprintWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlayWalkFPS(Model& weapon, Camera& cam, float dt)
{
	const float MAX_Z_OFFSET = m_originalWeaponZOffset - 0.4f;
	const float MIN_Z_OFFSET = m_originalWeaponZOffset;
	m_defWeaponYOffset = -2.5f;

	// Rotate the weapon back to 190 degrees if it isn't at 190 degrees 
	if (m_defWeaponRotation >= WEAPON_ROTATION_190_DEG)
	{
		m_defWeaponRotation -= 7.0f * dt;

		if (m_defWeaponRotation <= WEAPON_ROTATION_190_DEG)
			m_defWeaponRotation = WEAPON_ROTATION_190_DEG;
	}

	if (m_defWeaponZOffset <= MAX_Z_OFFSET)
		m_weaponMoveForward = false;
	else if (m_defWeaponZOffset >= MIN_Z_OFFSET)
		m_weaponMoveForward = true;

	if (m_weaponMoveForward)
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

	weapon.Draw(cam, glm::vec3(1.7f, -2.0f, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlayIdleFPS(Model& weapon, Camera& cam, float dt)
{
	// Bring weapon back to original Z position
	if (m_defWeaponZOffset <= m_originalWeaponZOffset)
	{
		m_defWeaponZOffset += 1.5f * dt;

		if (m_defWeaponZOffset >= m_originalWeaponZOffset)
			m_defWeaponZOffset = m_originalWeaponZOffset;
	}
	else
	{
		m_defWeaponZOffset -= 1.5f * dt;

		if (m_defWeaponZOffset <= m_originalWeaponZOffset)
			m_defWeaponZOffset = m_originalWeaponZOffset;
	}

	// Rotate the weapon back to 190 degrees if it isn't at 190 degrees 
	if (m_defWeaponRotation >= WEAPON_ROTATION_190_DEG)
	{
		m_defWeaponRotation -= 7.0f * dt;

		if (m_defWeaponRotation <= WEAPON_ROTATION_190_DEG)
			m_defWeaponRotation = WEAPON_ROTATION_190_DEG;
	}

	const float MAX_Y_OFFSET = -2.0f;
	const float MIN_Y_OFFSET = -2.1f;

	// Simulate breathing
	if (m_weaponMoveUp)
	{
		m_idleYOffset += 0.05f * dt;

		if (m_idleYOffset >= MAX_Y_OFFSET)
		{
			m_weaponMoveUp = false;
			m_idleYOffset = MAX_Y_OFFSET;
		}
	}
	else
	{
		m_idleYOffset -= 0.05f * dt;

		if (m_idleYOffset <= MIN_Y_OFFSET)
		{
			m_weaponMoveUp = true;
			m_idleYOffset = MIN_Y_OFFSET;
		}
	}

	weapon.Draw(cam, glm::vec3(1.7f, m_idleYOffset, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlayFireFPS(Model& weapon, Camera& cam, float dt)
{
	const float MAX_Z_OFFSET = -2.5f;
	const float MIN_Z_OFFSET = -1.5f;
	m_defWeaponYOffset = -2.5f;

	// Rotate the weapon back to 190 degrees if it isn't at 190 degrees 
	if (m_defWeaponRotation >= WEAPON_ROTATION_190_DEG)
	{
		m_defWeaponRotation -= 7.0f * dt;

		if (m_defWeaponRotation <= WEAPON_ROTATION_190_DEG)
			m_defWeaponRotation = WEAPON_ROTATION_190_DEG;
	}

	if (m_defWeaponZOffset <= MAX_Z_OFFSET)
		m_weaponMoveForward = false;
	else if (m_defWeaponZOffset >= MIN_Z_OFFSET)
		m_weaponMoveForward = true;

	if (m_weaponMoveForward)
	{
		m_defWeaponZOffset -= 10.0f * dt;

		// Prevent weapon from going too far into the world
		if (m_defWeaponZOffset <= MAX_Z_OFFSET)
			m_defWeaponZOffset = MAX_Z_OFFSET;
	}
	else
	{
		m_defWeaponZOffset += 10.0f * dt;

		// Prevent weapon from going too far behind the view
		if (m_defWeaponZOffset >= MIN_Z_OFFSET)
			m_defWeaponZOffset = MIN_Z_OFFSET;
	}

	weapon.Draw(cam, glm::vec3(1.7f, m_idleYOffset, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlayReloadFPS(Model& weapon, Camera& cam, float dt)
{
	const float MAX_ROTATION = 4.0f;
	const float MAX_Y_OFFSET = -2.2f;
	const float MIN_Y_OFFSET = -2.3f;

	m_defWeaponRotation += 5.0f * dt;

	if (m_defWeaponRotation >= MAX_ROTATION)
		m_defWeaponRotation = MAX_ROTATION;

	if (m_weaponMoveUp)
	{
		m_defWeaponYOffset += 0.2f * dt;

		if (m_defWeaponYOffset >= MAX_Y_OFFSET)
		{
			m_weaponMoveUp = false;
			m_defWeaponYOffset = MAX_Y_OFFSET;
		}
	}
	else
	{
		m_defWeaponYOffset -= 0.2f * dt;

		if (m_defWeaponYOffset <= MIN_Y_OFFSET)
		{
			m_weaponMoveUp = true;
			m_defWeaponYOffset = MIN_Y_OFFSET;
		}
	}

	weapon.Draw(cam, glm::vec3(1.7f, m_defWeaponYOffset, m_defWeaponZOffset), glm::vec3(-0.2f, 1.0f, -0.2f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}

void Animation::PlaySwapTwoWeapons(Model& weaponA, Model& weaponB, Camera& cam, float dt, bool& swapped)
{
	const float MAX_ROTATION = 4.5f;
	const float MAX_Y_OFFSET = -2.0f;
	const float MIN_Y_OFFSET = -2.5f;

	if (!m_swapping)
	{
		m_defWeaponRotation += 7.0f * dt;

		if (m_defWeaponRotation >= MAX_ROTATION)
			m_defWeaponRotation = MAX_ROTATION;

		m_defWeaponYOffset -= 7.0f * dt;
		weaponA.Draw(cam, glm::vec3(1.7f, m_defWeaponYOffset, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);

		if (m_defWeaponYOffset < -6.0f)
		{
			m_swapping = true;
		}
	}
	else if (m_swapping)
	{
		// Bring weapon back to original Z position
		if (m_defWeaponZOffset <= m_originalWeaponZOffset)
		{
			m_defWeaponZOffset += 1.5f * dt;

			if (m_defWeaponZOffset >= m_originalWeaponZOffset)
				m_defWeaponZOffset = m_originalWeaponZOffset;
		}
		else
		{
			m_defWeaponZOffset -= 1.5f * dt;

			if (m_defWeaponZOffset <= m_originalWeaponZOffset)
				m_defWeaponZOffset = m_originalWeaponZOffset;
		}

		if (m_defWeaponYOffset < -2.0f)
		{
			// Rotate the weapon back to 190 degrees if it isn't at 190 degrees 
			if (m_defWeaponRotation >= WEAPON_ROTATION_190_DEG)
			{
				m_defWeaponRotation -= 7.0f * dt;

				if (m_defWeaponRotation <= WEAPON_ROTATION_190_DEG)
					m_defWeaponRotation = WEAPON_ROTATION_190_DEG;
			}

			m_defWeaponYOffset += 7.0f * dt;
			weaponB.Draw(cam, glm::vec3(1.7f, m_defWeaponYOffset, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
		}
		else
		{
			swapped = true;
			m_swapping = false;
		}
	}
}

void Animation::PlayFreezeFPS(Model& weapon, Camera& cam, float dt)
{
	//weapon.Draw(cam, glm::vec3(1.7f, -2.0f, m_defWeaponZOffset), glm::vec3(0.0f, 1.0f, 0.0f), m_defWeaponRotation, glm::vec3(1.0f, 1.0f, 1.0f), true);
}