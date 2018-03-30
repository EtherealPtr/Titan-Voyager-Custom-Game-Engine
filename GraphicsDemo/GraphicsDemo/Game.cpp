#include "Game.h"

Game::Game()
{
	m_gameState = GameState::PLAY;
}

Game::~Game()
{
}

void Game::Run()
{
	InitMeshes();
	GameLoop();
}

void Game::InitMeshes()
{
	Renderer::GetInstance().InitMesh(TRIANGLE, "wall");
	Renderer::GetInstance().InitMesh(QUAD, "mario");
}

void Game::GameLoop()
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