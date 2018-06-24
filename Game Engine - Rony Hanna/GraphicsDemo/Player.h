#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"
#include "Animation.h"

class Player
{
public:
	Player();
	~Player();

	void ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events);
	void Update(Model& weapon, Camera& cam, float dt, std::vector<SDL_Event> events);

	void SetSprinting(bool sprinting) { m_sprinting = sprinting; }
	void SetWalking(bool walking) { m_walking = walking; }

	bool IsPlayerWalking();

private:
	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];

	bool m_walking;
	bool m_sprinting;

	Animation m_animationComponent;
};

#endif // !__PLAYER_H__