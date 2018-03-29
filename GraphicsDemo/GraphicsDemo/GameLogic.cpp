#include "GameLogic.h"

GameLogic::GameLogic()
{
	m_gameState = GameState::PLAY;
}

GameLogic::~GameLogic()
{
}

void GameLogic::Run()
{
	InitMeshes();
	GameLoop();
}

void GameLogic::InitMeshes()
{
	Renderer::GetInstance().InitMesh(TRIANGLE, "mario");
}

void GameLogic::GameLoop()
{
	while (m_gameState != GameState::EXIT)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int action = m_input.ProcessInput();

		if (action == (int)GameState::EXIT)
			m_gameState = GameState::EXIT;

		Renderer::GetInstance().RenderMeshes();

		SDL_GL_SwapWindow(Renderer::GetInstance().GetAppWindow());
	}
}