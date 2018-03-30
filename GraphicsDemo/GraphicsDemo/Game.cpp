#include "Game.h"

Game::Game() :
	m_deltaTime(0.0f),
	m_gameState(GameState::PLAY) 
{
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
	float lastFrame = 0.0;

	while (m_gameState != GameState::EXIT)
	{
		float currFrame = SDL_GetTicks();
		m_deltaTime = (currFrame - lastFrame) / 1000;
		lastFrame = currFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput();
		Update();

		m_camera.Update();
		//Renderer::GetInstance().RenderMeshes(m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix());
		Renderer::GetInstance().GetComponent("mario").Draw(m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix(), glm::vec3(-1.0f, 0.0f, 0.0f));
		Renderer::GetInstance().GetComponent("wall").Draw(m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix(), glm::vec3(0.0f, 0.0f, 0.0f));

		SDL_GL_SwapWindow(Renderer::GetInstance().GetAppWindow());
		SDL_Delay(1);
	}
}

void Game::Update()
{
#pragma region // CAMERA_MOVEMENT
	if (m_bCamMovements[CAM_FORWARD] == true)
		m_camera.MoveForward(m_deltaTime);
	if (m_bCamMovements[CAM_BACKWARD] == true)
		m_camera.MoveBackward(m_deltaTime);
	if (m_bCamMovements[CAM_LEFT] == true)
		m_camera.StrafeLeft(m_deltaTime);
	if (m_bCamMovements[CAM_RIGHT] == true)
		m_camera.StrafeRight(m_deltaTime);
	if (m_bCamMovements[CAM_RISE] == true)
		m_camera.Rise(m_deltaTime);
	if (m_bCamMovements[CAM_FALL] == true)
		m_camera.Fall(m_deltaTime);
#pragma endregion

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
					m_bCamMovements[CAM_FORWARD] = true;
					break;

				case SDLK_s:
					m_bCamMovements[CAM_BACKWARD] = true;
					break;

				case SDLK_a:
					m_bCamMovements[CAM_LEFT] = true;
					break;

				case SDLK_d:
					m_bCamMovements[CAM_RIGHT] = true;
					break;

				case SDLK_r:
					m_bCamMovements[CAM_RISE] = true;
					break;

				case SDLK_f:
					m_bCamMovements[CAM_FALL] = true;
					break;

				default: break;
			}

			break;
		}

		case SDL_KEYUP:
		{
			switch (m_event.key.keysym.sym)
			{
				case SDLK_z:
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					break;

				case SDLK_w:
					m_bCamMovements[CAM_FORWARD] = false;
					break;

				case SDLK_s:
					m_bCamMovements[CAM_BACKWARD] = false;
					break;

				case SDLK_a:
					m_bCamMovements[CAM_LEFT] = false;
					break;

				case SDLK_d:
					m_bCamMovements[CAM_RIGHT] = false;
					break;

				case SDLK_r:
					m_bCamMovements[CAM_RISE] = false;
					break;

				case SDLK_f:
					m_bCamMovements[CAM_FALL] = false;
					break;

				default: break;
				}

			break;
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