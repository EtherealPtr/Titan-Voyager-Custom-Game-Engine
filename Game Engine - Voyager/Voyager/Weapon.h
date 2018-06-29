#pragma once
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Animation.h"

class Weapon
{
public:
	Weapon();
	~Weapon();

	void Init(GLchar* path, Camera& camera, char* vs, char* fs);

	void Fire(Model& weapon, Camera& cam, float dt, bool& firing, bool& reloading);
	void Reload(Model& weapon, Camera& cam, float dt, bool& reloading);

	void SetAmmoCount(int maxAmmo) { m_maxAmmo = maxAmmo; }
	inline int GetAmmoCount() { return m_ammoCount; }

	inline Animation& GetAnimComponent() { return m_animator; }
	inline Model& GetModel() { return m_model; }

private:
	float m_fireTimer, m_fireRate, m_reloadTimer;
	int m_ammoCount, m_maxAmmo;
	Animation m_animator;
	Model m_model;
};

#endif // !__WEAPON_H__