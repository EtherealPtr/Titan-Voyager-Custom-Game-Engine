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
	void Configure(int maxAmmo, float fireRate, float reloadTime, int damage);
	void Update(Camera& cam, float dt);
	void Fire(Model& weapon, Camera& cam, float dt, bool& firing, bool& reloading);
	void Reload(Model& weapon, Camera& cam, float dt, bool& reloading);
	void RestartWeapon();

	inline void SetMaxAmmo(int maxAmmo) { m_maxAmmo = maxAmmo; }
	inline int GetMaxAmmoCount() { return m_maxAmmo; }
	inline int GetAmmoCount() { return m_ammoCount; }

	inline void SetFireRate(float fireRate) { m_fireRate = fireRate; }
	inline float GetFireRate() { return m_fireRate; }

	inline void SetReloadTime(float reloadTime) { m_maxReloadTimer = reloadTime; }
	inline float GetReloadTime() { return m_maxReloadTimer; }

	inline void SetDamage(int dmg) { m_damage = dmg; }
	inline int GetDamage() { return m_damage; }

	inline Animation& GetAnimComponent() { return m_animator; }
	inline Model& GetModel() { return m_model; }

private:
	float m_currFireRateTime, m_fireRate, m_currReloadTime, m_maxReloadTimer;
	int m_ammoCount, m_maxAmmo, m_damage;
	Animation m_animator;
	Model m_model;
};

#endif // !__WEAPON_H__