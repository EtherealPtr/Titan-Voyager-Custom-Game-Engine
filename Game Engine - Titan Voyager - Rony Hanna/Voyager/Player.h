#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Physics.h"
#include "Weapon.h"
#include "SpotLight.h"

class Player
{
public:
	~Player();

	static Player& GetInstance()
	{
		static Player instance;
		return instance;
	}

	Player(Player const&) = delete;
	void operator=(Player const&) = delete;

	void Init(Camera& cam, glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 1.0f));
	void ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events);
	void Update(Camera& cam, Terrain& terrain, float dt, std::vector<SDL_Event> events);
	void Switch();

	void ReduceHealth(float amount)		{ m_health -= amount; }
	void Respawn(Camera& cam);
	
	int GetHealth()						{ return m_health; }
	bool IsPlayerJumping()				{ return m_jumping; }
	bool IsPlayerFiring()				{ return m_muzzleFlash; }
	bool IsPlayerAiming()				{ return m_sniperAiming; }
	bool IsUsingSniper()			    { return m_sniperEquipped; }
	bool IsPlayerWalking();
	bool IsPlayerDead()					{ return m_dead; }
	Animation& GetAnimationComponent()	{ return m_animationComponent; }
	Weapon& GetCurrWeapon()				{ return *m_currWeapon; }
	SpotLight* GetSpotLight()			{ return m_spotLight;}

private:
	Player();

	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, CAM_JUMP, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];

	bool m_walking, m_sprinting, m_jumping, m_isInAir, m_firing, m_reloading, m_muzzleFlash, m_usingAR, m_swapping, m_swapped;
	bool m_sniperAiming, m_sniperEquipped, m_toggleFlashlight, m_dead;
	int m_health;
	float m_jumpHeight, m_upwardSpeed;

	glm::vec3 m_pos;
	SpotLight* m_spotLight;
	Animation m_animationComponent;
	Weapon* m_currWeapon, *m_assaultRifle, *m_sniperRifle;

private:
	void StopMoving();
};

#endif // !__PLAYER_H__