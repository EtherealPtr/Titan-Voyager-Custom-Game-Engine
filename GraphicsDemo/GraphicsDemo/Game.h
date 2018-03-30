#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include "Renderer.h"
#include "Camera.h"

enum class GameState { MAIN_MENU, PLAY, EXIT, TOTAL_STATES };
enum class Controller { W_DOWN = 50, S_DOWN, A_DOWN, D_DOWN, W_UP, S_UP, A_UP, D_UP };

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

	GameState m_gameState;
	Camera m_camera;

	double m_deltaTime = 0.0f;	
	double m_lastFrame = 0.0f; 
};

#endif // !__GAME_H__