#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include "Renderer.h"
#include "Camera.h"
#include "Model.h"

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

	enum class GameState { MAIN_MENU = 100, PLAY, EXIT, TOTAL_STATES };
	GameState m_gameState;
private:
	float m_deltaTime;
	//Model m_model[2];
	Model m_asteroid;
};

#endif // !__GAME_H__