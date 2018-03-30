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
		float currFrame = SDL_GetTicks();
		m_deltaTime = (currFrame - m_lastFrame) / 1000;
		m_lastFrame = currFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput();

		m_camera.Update();
		Renderer::GetInstance().RenderMeshes(m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix());

		SDL_GL_SwapWindow(Renderer::GetInstance().GetAppWindow());
		SDL_Delay(1);
	}
}

void Game::ProcessInput()
{
	SDL_Event m_event;

	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;

			// KEYBOARD_INPUT
		case SDL_KEYDOWN:
		{
			switch (m_event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_gameState= GameState::EXIT;
				break;

			case SDLK_z:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;

			case SDLK_w:
				m_camera.MoveForward(m_deltaTime);
				break;

			case SDLK_s:
				m_camera.MoveBackward(m_deltaTime);
				break;

			case SDLK_a:
				m_camera.StrafeLeft(m_deltaTime);
				break;

			case SDLK_d:
				m_camera.StrafeRight(m_deltaTime);
				break;

			case SDLK_r:
				m_camera.Rise(m_deltaTime);
				break;

			case SDLK_f:
				m_camera.Fall(m_deltaTime);
				break;

			default: break;
			}
		}

		case SDL_KEYUP:
		{
			switch (m_event.key.keysym.sym)
			{
			case SDLK_w:
				 
				break;

			case SDLK_s:
				break;

			case SDLK_a:
				break;

			case SDLK_d:
				break;

			default: break;
			}
		}
		// KEYBOARD_INPUT END


		case SDL_MOUSEMOTION:
		{
			m_camera.MouseUpdate(glm::vec2(m_event.motion.x, m_event.motion.y), m_deltaTime);
			break;
		}

		default:
			break;
		}

	}
}