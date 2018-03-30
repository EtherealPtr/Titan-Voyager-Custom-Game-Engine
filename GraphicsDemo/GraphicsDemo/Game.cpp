#include "Game.h"

Game::Game()
{
	m_gameState = GameState::PLAY;
	//m_camera.InitCameraOrthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 600.0f);
	m_camera.InitCameraPerspective(80.0f, 800.0f / 600.0f, 0.1f, 500.0f);
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

		Update();

		m_camera.Update();
		Renderer::GetInstance().RenderMeshes(m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix());

		SDL_GL_SwapWindow(Renderer::GetInstance().GetAppWindow());
	}
}

void Game::Update()
{
	int action = m_input.ProcessInput();

	switch (action)
	{
	case (int)GameState::EXIT:
		m_gameState = GameState::EXIT;
		break;

	case (int)Controller::W_DOWN:
		m_camera.GetCameraPos() += 0.05f * m_camera.GetCameraForward();
		break;
	
	case (int)Controller::S_DOWN:
		m_camera.GetCameraPos() -= 0.05f * m_camera.GetCameraForward();
		break;

	case (int)Controller::A_DOWN:
		m_camera.GetCameraPos() -= 0.05f * m_camera.GetCameraRightVector();
		break;

	case (int)Controller::D_DOWN:
		m_camera.GetCameraPos() += 0.05f * m_camera.GetCameraRightVector();
		break;

	case (int)Controller::W_UP:
		break;

	case (int)Controller::S_UP:
		break;

	case (int)Controller::A_UP:
		break;

	case (int)Controller::D_UP:
		break;

	default: break;
	}
}