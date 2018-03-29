#pragma once
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Renderer.h"
#include "GameLogic.h"
#include "ResourceManager.h"

class Engine
{
public:
	Engine();
	~Engine();

	void Run();

private:
	void LoadApplicationResources();

	GameLogic m_game;
};

#endif // !__ENGINE_H__