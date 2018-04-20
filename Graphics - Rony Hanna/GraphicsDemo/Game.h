#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include "Renderer.h"
#include "Camera.h"
#include "Model.h"
#include "Terrain.h"
#include "Light.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void InitMeshes();
	void ProcessInput();
	void GameLoop();
	void Update();

	Camera m_camera;

private:
	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];
	
	enum { SATURN_RINGS = 1, SPACE_CUBE, SKYBOX, SATURN, MARS, MERCURY, NEPTUNE, EARTH };
	
	enum class GameState { MAIN_MENU = 100, PLAY, EXIT, TOTAL_STATES };
	GameState m_gameState;
	float m_deltaTime;

private:
	Model m_asteroid;
	Model m_aircraft;
	Terrain m_terrain;
	Light m_light;
};

#endif // !__GAME_H__