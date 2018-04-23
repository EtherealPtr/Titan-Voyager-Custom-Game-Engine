#include "Game.h"

Game::Game() :
	m_deltaTime(0.0f),
	m_gameState(GameState::PLAY),
	m_spaceScene(false)
{
	m_camera.InitCameraPerspective(80.0f, 1440.0f / 900.0f, 0.1f, 5000.0f);
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
	int id = 0;
	std::vector<char*> defShaders{ "res/Shaders/DefaultVertexShader.vs", "res/Shaders/DefaultFragmentShader.fs" };
	std::vector<char*> skyboxShaders{ "res/Shaders/SkyboxVertexShader.vs", "res/Shaders/SkyboxFragmentShader.fs" };
	std::vector<char*> normalMappingShaders{ "res/Shaders/NormalMapping.vs", "res/Shaders/NormalMapping.fs" };
	
	Renderer::GetInstance().InitMesh(QUAD, "saturnRings", ++id, defShaders, glm::vec3(200.0f, 160.0f, -500.0f), glm::vec3(-65.0f, 0.0f, 0.0f), glm::vec3(330.0f, 330.0f, 330.0f));
	Renderer::GetInstance().InitMesh(CUBE, "cubeTex", ++id, normalMappingShaders, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f), "cubeTexNormalMap");
	//Renderer::GetInstance().InitMesh(CUBE, "cubeTex", ++id, defShaders, glm::vec3(3.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer::GetInstance().InitMesh(CUBE, "skybox", ++id, skyboxShaders, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3000.0f, 3000.0f, 3000.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "saturn", ++id, defShaders, glm::vec3(200.0f, 150.0f, -500.0f), glm::vec3(25.0f, 90.0f, 0.0f), glm::vec3(55.0f, 55.0f, 55.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "mars", ++id, defShaders, glm::vec3(-70.0f, 50.0f, -70.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(6.36f, 6.36f, 6.36f));
	Renderer::GetInstance().InitMesh(SPHERE, "mercury", ++id, defShaders, glm::vec3(50.0f, 45.0f, -60.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(4.56f, 4.56f, 4.56f));
	Renderer::GetInstance().InitMesh(SPHERE, "neptune", ++id, defShaders, glm::vec3(-200.0f, -70.0f, -180.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(40.0f, 40.0f, 40.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "earth", ++id, defShaders, glm::vec3(0.0f, 0.0f, -60.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(12.0f, 12.0f, 12.0f));

	m_terrain.InitTerrain("res/Shaders/TerrainVertexShader.vs", "res/Shaders/TerrainFragmentShader.fs");
	m_terrain.CreateTerrainWithPerlinNoise();
	m_light.SetPos(glm::vec3(0.0f, -139.0f, 0.0f));
	//m_aircraft.Init("res/Models3D/Walkyrie/object.obj", m_camera, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs", false);
	//m_asteroid.Init("res/Models3D/Rock/rock.obj", m_camera, "res/Shaders/InstancingVert.vs", "res/Shaders/InstancingFrag.fs", true);
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
		m_camera.UpdateLookAt();
		m_light.SetPos(glm::vec3(m_camera.GetCameraPos().x, m_camera.GetCameraPos().y, m_camera.GetCameraPos().z));
		//m_aircraft.Draw(m_camera);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		if (m_spaceScene)
		{
			Renderer::GetInstance().GetComponent(SPACE_CUBE).Draw(m_camera, glm::vec3(m_camera.GetCameraPos().x, m_camera.GetCameraPos().y, m_camera.GetCameraPos().z), true);
			Renderer::GetInstance().GetComponent(SPACE_CUBE).GetTransformComponent().GetRot().y += 6.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(SPACE_CUBE).GetTransformComponent().GetRot().x += 6.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(3).Draw(m_camera, glm::vec3(m_camera.GetCameraPos().x, m_camera.GetCameraPos().y, m_camera.GetCameraPos().z), false);
			//Renderer::GetInstance().GetComponent(3).GetTransformComponent().GetRot().y += 6.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(3).GetTransformComponent().GetRot().x += 6.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(SATURN_RINGS).Draw(m_camera);
			//
			//// Planets
			//Renderer::GetInstance().GetComponent(SATURN).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(SATURN).Draw(m_camera);
			//
			//Renderer::GetInstance().GetComponent(MARS).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(MARS).Draw(m_camera);
			//
			//Renderer::GetInstance().GetComponent(MERCURY).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(MERCURY).Draw(m_camera);
			//
			//Renderer::GetInstance().GetComponent(NEPTUNE).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(NEPTUNE).Draw(m_camera);
			//
			//Renderer::GetInstance().GetComponent(EARTH).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			//Renderer::GetInstance().GetComponent(EARTH).Draw(m_camera);
		}

		//m_asteroid.DrawInstanced(m_camera);

		glDisable(GL_CULL_FACE);

		m_terrain.Draw(m_camera, m_light.GetPos());

		Renderer::GetInstance().GetComponent(SKYBOX).Draw(m_camera);

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
				m_gameState = GameState::EXIT;
				break;

			case SDLK_SPACE:
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
			case SDLK_SPACE:
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

			case SDLK_z:
				// Temp (Remove this later)
				Renderer::GetInstance().GetComponent(SKYBOX).GetTextureComponent().GenerateSkybox(6, 12);
				m_spaceScene = false;
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