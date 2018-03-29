#pragma once
#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include <iostream>
#include "Input.h"
#include "Renderer.h"
#include "Camera.h"

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	void Run();

private:
	void InitMeshes();
	void GameLoop();

	GameState m_gameState;
	Input m_input;
	Camera m_camera;
};

#endif // !__GAMELOGIC_H__