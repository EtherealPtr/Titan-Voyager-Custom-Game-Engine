#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Model.h"
#include "Camera.h"

class Animation
{
public:
	Animation();
	~Animation();

	void PlaySprintFPS(Model& weapon, Camera& cam, float dt);
	void PlayWalkFPS(Model& weapon, Camera& cam, float dt);
	void PlayIdleFPS(Model& weapon, Camera& cam, float dt);
	void PlayFireFPS(Model& weapon, Camera& cam, float dt);
	void PlayReloadFPS(Model& weapon, Camera& cam, float dt);
	void PlaySwapTwoWeapons(Model& weaponA, Model& weaponB, Camera& cam, float dt, bool& swapped);
	void PlayFreezeFPS(Model& weapon, Camera& cam, float dt);

	void SetWeaponZOffset(float zOffset);
	void SetWeaponYOffset(float yOffset) { m_defWeaponYOffset = yOffset; }
	void SetSprintWeaponZOffset(float zOffset) { m_sprintWeaponZOffset = zOffset; }

private:
	float m_defWeaponRotation;
	float m_defWeaponZOffset;
	float m_sprintWeaponZOffset;
	float m_originalWeaponZOffset;
	float m_defWeaponYOffset;
	bool m_weaponMoveUp;
	bool m_weaponMoveForward;
	bool m_swapping;
	float m_idleYOffset;
};

#endif // !__ANIMATION_H__