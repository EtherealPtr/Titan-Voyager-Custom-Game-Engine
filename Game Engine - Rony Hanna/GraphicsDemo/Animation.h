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

	void PlaySprintFPS(Model& gun, Camera& cam, float dt);
	void PlayWalkFPS(Model& gun, Camera& cam, float dt);
	void PlayIdleFPS(Model& gun, Camera& cam, float dt);

private:
	float m_defWeaponRotation;
	float m_defWeaponZOffset;
	float m_defWeaponYOffset;
	bool weaponMoveUp;
	bool weaponMoveForward;
	float m_idleYOffset;
};

#endif // !__ANIMATION_H__