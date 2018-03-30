#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include "Input.h"
#include "Renderer.h"
#include "Camera.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void InitMeshes();
	void GameLoop();
	void Update();

	GameState m_gameState;
	Input m_input;
	Camera m_camera;
};

#endif // !__GAME_H__