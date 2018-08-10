#include "Game.h"
#include "Utils.h"

std::vector<SDL_Event>& GetFrameEvents()
{
	static std::vector<SDL_Event> frame_events;
	return frame_events;
}

Game::Game() :
	m_deltaTime(0.0f),
	m_gameState(GameState::MAIN_MENU),
	m_sniperScope(false),
	m_dataTransmitTimer(0.0f)
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_camera.InitCameraPerspective(80.0f, 1440.0f / 900.0f, 0.1f, 5000.0f);
	m_cameraHUD.InitCameraOrthographic(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 100.0f);
}

Game::~Game()
{
	// Deallocate all enemies stored on the heap (or freestore)
	for (auto& i : m_enemies)
	{
		delete i;
		i = nullptr;
	}
}

void Game::Run()
{
	InitMeshes();
	InitLights();
	//InitDebugger();
	InitText();
	InitMultiRenderTarget();
	InitCloth();
	InitPlayer();
	GameLoop();
}

void Game::InitMeshes()
{
	int id = 0;
	std::vector<char*> defShader{ "res/Shaders/DefaultVertexShader.vs", "res/Shaders/DefaultFragmentShader.fs" };
	std::vector<char*> unlitShader{ "res/Shaders/Unlit.vs", "res/Shaders/Unlit.fs" };
	std::vector<char*> skyboxShader{ "res/Shaders/SkyboxVertexShader.vs", "res/Shaders/SkyboxFragmentShader.fs" };
	std::vector<char*> normalMappingShader{ "res/Shaders/NormalMapping.vs", "res/Shaders/NormalMapping.fs" };
	std::vector<char*> hudShader{ "res/Shaders/HUD.vs", "res/Shaders/HUD.fs" };
	std::vector<char*> muzzleFlashShader{ "res/Shaders/Muzzle Flash Shader/VertexShaderMuzzleFlash.vs", "res/Shaders/Muzzle Flash Shader/FragmentShaderMuzzleFlash.fs" };
	std::vector<char*> enemyShader{ "res/Shaders/EnemyVertexShader.vs", "res/Shaders/EnemyFragmentShader.fs" };
	std::vector<char*> postProcessingShader{ "res/Shaders/PostProcessingVertexShader.vs", "res/Shaders/PostProcessingFragmentShader.fs" };

	// Initialize the meshes
	Renderer::GetInstance().InitMesh(QUAD, "saturnRings", ++id, defShader, glm::vec3(200.0f, 360.0f, -700.0f), glm::vec3(-65.0f, 0.0f, 0.0f), glm::vec3(640.0f, 640.0f, 640.0f));
	Renderer::GetInstance().InitMesh(CUBE, "cubeTex", ++id, normalMappingShader, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f), "cubeTexNormalMap");
	Renderer::GetInstance().InitMesh(CUBE, "skybox", ++id, skyboxShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3000.0f, 3000.0f, 3000.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "saturn", ++id, defShader, glm::vec3(200.0f, 350.0f, -700.0f), glm::vec3(25.0f, 90.0f, 0.0f), glm::vec3(95.0f, 95.0f, 95.0f));
	Renderer::GetInstance().InitMesh(QUAD, "crossHair", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer::GetInstance().InitMesh(QUAD, "ammo", ++id, hudShader, glm::vec3(-14.0f, -18.0f, 0.0f), glm::vec3(0.0f, 0.0f, 105.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	Renderer::GetInstance().InitMesh(QUAD, "health", ++id, hudShader, glm::vec3(-18.0f, -18.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(0.95f, 1.2f, 1.0f));
	Renderer::GetInstance().InitMesh(QUAD, "muzzleFlash", ++id, muzzleFlashShader, glm::vec3(2.0f, -2.5f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer::GetInstance().InitMesh(QUAD, "sniperScope", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 1.0f));
	Renderer::GetInstance().InitMesh(QUAD, "cubeTex", ++id, postProcessingShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "drone", ++id, unlitShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "shockwave", ++id, unlitShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f));
	Renderer::GetInstance().InitMesh(QUAD, "mainMenu", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f));
	Renderer::GetInstance().InitMesh(QUAD, "indicator", ++id, hudShader, glm::vec3(-30.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

	// Enemy ID registeration [100, 130] inclusively 
	id = 100;
	for (unsigned int i = 0; i < 1; ++i)
	{
		Renderer::GetInstance().InitMesh(SPHERE, "enemySphere", id++, enemyShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Enemy* enemy = new Enemy(m_camera);
		m_enemies.push_back(enemy);
	}

	// Initialize game models
	m_terrain.InitTerrain("res/Shaders/TerrainVertexShader.vs", "res/Shaders/TerrainFragmentShader.fs");
	m_terrain.CreateTerrainWithPerlinNoise();

	m_asteroid.Init("res/Models3D/Rock/LowPolyRock.dae", m_camera, "res/Shaders/InstancingVert.vs", "res/Shaders/InstancingFrag.fs", true);

	m_flagPole.Init("res/Models3D/FlagPole/Pole.obj", m_camera, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs", false);
	m_flagPole.SetSpotlight(true);

	for (auto i = 0; i < 5; ++i)
	{
		// Create mountain rock
		Model mountainRock;
		mountainRock.Init("res/Models3D/Rock/LowPolyRock.dae", m_camera, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs", false);

		// Set rock's spotlight calculations to true so that it is affected by the player's "flashlight" 
		mountainRock.SetSpotlight(true);

		// Set rock's position, rotation, and scale
		switch (i)
		{
		case 0: mountainRock.SetTransform(glm::vec3(30.0f, 60.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, glm::vec3(20.0f, 36.0f, 20.0f)); break;
		case 1: mountainRock.SetTransform(glm::vec3(512.0f, 63.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, glm::vec3(20.0f, 36.0f, 20.0f)); break;
		case 2: mountainRock.SetTransform(glm::vec3(750.0f, 63.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 100.0f, glm::vec3(20.0f, 36.0f, 20.0f)); break;
		case 3: mountainRock.SetTransform(glm::vec3(30.0f, 60.0f, 750.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, glm::vec3(20.0f, 36.0f, 20.0f)); break;
		case 4: mountainRock.SetTransform(glm::vec3(750.0f, 63.0f, 750.0f), glm::vec3(0.0f, 1.0f, 0.0f), 100.0f, glm::vec3(20.0f, 36.0f, 20.0f)); break;
		default: break;
		}

		// Store newly created mountain rock in vector
		m_mountainRocks.push_back(mountainRock);
	}
}

void Game::InitLights()
{
	m_dirLight.Configure(glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f));
	m_dirLight.SetDirection(glm::vec3(0.2f, 1.0f, 0.5f));
	m_dirLight.SetColour(glm::vec3(0.97f, 0.88f, 0.70f));

	m_pointLight.Configure(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.045f, 0.0075f);
	m_pointLight.SetPosition(glm::vec3(256.0f, 50.0f, 300.0f));
	m_pointLight.SetLightColour(glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::InitDebugger()
{
	m_debugger.Init(m_camera);
}

void Game::InitText()
{
	Text ammoText;
	ammoText.Configure("res/Fonts/Roboto-BoldItalic.ttf");
	ammoText.SetText("35");
	ammoText.SetScale(0.79f);
	ammoText.SetSpacing(0.79f);
	ammoText.SetPosition(glm::vec2(240.0f, 33.0f));
	m_texts.push_back(ammoText);

	Text healthText;
	healthText.Configure("res/Fonts/Roboto-BoldItalic.ttf");
	healthText.SetText("100");
	healthText.SetScale(0.79f);
	healthText.SetSpacing(0.79f);
	healthText.SetPosition(glm::vec2(110.0f, 33.0f));
	m_texts.push_back(healthText);

	Text dataTransmissionText;
	dataTransmissionText.Configure("res/Fonts/Roboto-BoldItalic.ttf");
	dataTransmissionText.SetText("100%");
	dataTransmissionText.SetScale(0.5f);
	dataTransmissionText.SetSpacing(0.7f);
	dataTransmissionText.SetPosition(glm::vec2(600.0f, 33.0f));
	m_texts.push_back(dataTransmissionText);
}

void Game::InitMultiRenderTarget()
{
	m_framebuffer.CreateFramebuffer();
}

void Game::InitCloth()
{
	m_flag.Configure(10, 6, 20, 17);
	m_flag.SetPos(glm::vec3(256.4f, m_terrain.GetHeightOfTerrain(256.0f, 300.0f) + 45.0f, 270.0f));
}

void Game::InitPlayer()
{
	Player::GetInstance().Init(m_camera, glm::vec3(256.0f, 0.0f, 300.0f));
}

void Game::GameLoop()
{
	float lastFrame = 0.0;

	while (m_gameState != GameState::EXIT)
	{
		float currFrame = SDL_GetTicks();
		m_deltaTime = (currFrame - lastFrame) / 1000;
		lastFrame = currFrame;

		SDL_Event _event;
		while (SDL_PollEvent(&_event) != 0)
		{
			GetFrameEvents().push_back(_event);
		}

		ProcessInput(GetFrameEvents());

		if (m_gameState == GameState::MAIN_MENU)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			UpdateMenu();
			Renderer::GetInstance().GetComponent(INDICATOR).Draw(m_cameraHUD);
			Renderer::GetInstance().GetComponent(MAIN_MENU).Draw(m_cameraHUD);
		}
		else
		{
			m_framebuffer.ActivateFramebuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			UpdateGame();
			RenderScene();

			m_framebuffer.DeactivateFramebuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).Draw(m_cameraHUD, glm::vec3(0.0f, 0.0f, 0.0f), false, nullptr, true, m_framebuffer.GetColorBufferTexture());
			Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().ActivateProgram();
			Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().SetBool("thunderstormEffect", false);
		}

		SDL_GL_SwapWindow(Renderer::GetInstance().GetAppWindow());
		SDL_Delay(1);
	}
}

void Game::RenderScene()
{
	// Draw flag (cloth simulation)
	m_flag.AddForce(glm::vec3(0.0f, -0.2f, 0.0f) * 0.25f);
	m_flag.WindForce(glm::vec3(0.7f, 0.1f, 0.2f) * 0.25f);
	m_flag.Update();
	m_flag.Draw(m_camera);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Draw flag pole 
	m_flagPole.Draw(m_camera, glm::vec3(256.0f, m_terrain.GetHeightOfTerrain(256.0f, 300.0f) + 10.0f, 270.0f));

	// Draw big rocks around the corners
	for (auto i : m_mountainRocks)
		i.Draw(m_camera, false);

	// Check if the player is aiming
	if (m_sniperScope)
	{
		// Render sniper scope
		Renderer::GetInstance().GetComponent(SNIPER_SCOPE).Draw(m_cameraHUD);

		// Zoom in and slow player speed and mouse sensitivity
		if (m_camera.GetCameraFOV() != 30.0f)
		{
			m_camera.InitCameraPerspective(30.0f, 1440.0f / 900.0f, 0.1f, 5000.0f);
			m_camera.SetCameraSpeed(10.0f);
			m_camera.SetCameraSensitivity(4.0f);
		}
	}
	else
	{
		Renderer::GetInstance().GetComponent(FPS_CROSSHAIR).Draw(m_cameraHUD);

		if (m_camera.GetCameraFOV() != 80.0f)
		{
			m_camera.InitCameraPerspective(80.0f, 1440.0f / 900.0f, 0.1f, 5000.0f);
			m_camera.SetCameraSpeed(20.0f);
			m_camera.SetCameraSensitivity(7.0f);
		}
	}

	Renderer::GetInstance().GetComponent(AMMO).Draw(m_cameraHUD);
	Renderer::GetInstance().GetComponent(HEALTH).Draw(m_cameraHUD);
	Renderer::GetInstance().GetComponent(SATURN_RINGS).Draw(m_camera);
	Renderer::GetInstance().GetComponent(SATURN).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
	Renderer::GetInstance().GetComponent(SATURN).Draw(m_camera, glm::vec3(-700.0f, 700.0f, 0.0f));

	m_asteroid.DrawInstanced(m_camera);

	glDisable(GL_CULL_FACE);

	//m_terrain.SetFog(m_atmosphere.GetDayTime() <= 0.15f ? true : false);
	m_terrain.Draw(m_camera, &m_dirLight, &m_pointLight, Player::GetInstance().GetSpotLight());

	Renderer::GetInstance().GetComponent(SKYBOX).GetTransformComponent().GetRot().y += 0.5f * m_deltaTime;
	Renderer::GetInstance().GetComponent(SKYBOX).Draw(m_camera);

	// Draw enemy units
	int enemyId = 100;
	for (auto& enemy : m_enemies)
	{
		enemy->Draw(enemyId, ENEMY_DRONE, ENEMY_BLAST);
		++enemyId;
	}

	// Text updates
	// [0] : Ammo, [1] : Health, [2] : Data transmission percentage
	m_texts[0].SetText(std::to_string(Player::GetInstance().GetCurrWeapon().GetAmmoCount()));
	m_texts[0].Render();

	m_texts[1].SetText(std::to_string(Player::GetInstance().GetHealth()));
	m_texts[1].Render();

	m_texts[2].SetText("Data transfer: " + std::to_string((int)m_dataTransmitTimer) + "%");
	m_texts[2].Render();
}

void Game::UpdateGame()
{
	m_camera.UpdateLookAt();
	Player::GetInstance().Update(m_camera, m_terrain, m_deltaTime, GetFrameEvents());
	
	if (Player::GetInstance().IsPlayerDead())
	{
		RestartGame();
	}

	if (Player::GetInstance().IsPlayerAiming())
		m_sniperScope = true;
	else
		m_sniperScope = false;

	// Update enemy units
	int enemyId = 100;
	for (auto i : m_enemies)
	{
		(*i).Update(m_terrain, m_camera, m_deltaTime);
		++enemyId;
	}

	// Update physics component
	Physics::GetInstance().Update(m_camera, m_deltaTime, GetFrameEvents(), m_enemies);

	// Update mountain rocks' fog effect 
	//if (m_atmosphere.GetDayTime() >= 0.15f && m_atmosphere.GetDayTime() <= 0.16f)
	//{
	//	for (auto i : m_mountainRocks)
	//	{
	//		i.GetShaderProgram().ActivateProgram();
	//		i.GetShaderProgram().SetBool("nightFog", true);
	//	}
	//}
	//else if (m_atmosphere.GetDayTime() < 0.15f && m_atmosphere.GetDayTime() >= 0.14f)
	//{
	//	for (auto i : m_mountainRocks)
	//	{
	//		i.GetShaderProgram().ActivateProgram();
	//		i.GetShaderProgram().SetBool("nightFog", false);
	//	}
	//}

	// Update atmosphere (thunderstorms)
	m_atmosphere.Update(m_deltaTime);

	if (m_atmosphere.GetFlashDuration() > 0)
	{
		if (m_atmosphere.GetThunderFlash())
		{
			--m_atmosphere.GetFlashDuration();
			m_atmosphere.SetThunderFlash(false);
			Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().ActivateProgram();
			Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().SetBool("thunderstormEffect", true);
		}

		m_atmosphere.GetFlashTimer() += Utils::GetInstance().RandomNumBetweenTwo(0.5f, 1.0f) * m_deltaTime;
		if (m_atmosphere.GetFlashTimer() > Utils::GetInstance().RandomNumBetweenTwo(0.05f, 0.1f))
		{
			m_atmosphere.GetThunderFlash() = !m_atmosphere.GetThunderFlash();
			m_atmosphere.GetFlashTimer() = 0.0f;
		}
	}

	// Update data transmitter 
	m_dataTransmitTimer += 0.39f * m_deltaTime;

	GetFrameEvents().clear();
}

void Game::UpdateMenu()
{
	GetFrameEvents().clear();
}

void Game::RestartGame()
{
	// Respawn the player
	Player::GetInstance().Respawn(m_camera);
}

void Game::ProcessInput(std::vector<SDL_Event>& events)
{
	for (auto i = events.begin(); i != events.end(); ++i)
	{
		switch (i->type)
		{
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;

			// KEYBOARD_INPUT
		case SDL_KEYDOWN:
		{
			switch (i->key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_gameState = GameState::EXIT;
				break;

			case SDLK_x:
				SDL_SetWindowFullscreen(Renderer::GetInstance().GetAppWindow(), SDL_WINDOW_FULLSCREEN);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;

			default: break;
			}

			break;
		}

		case SDL_KEYUP:
		{
			switch (i->key.keysym.sym)
			{
			case SDLK_x:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;

			case SDLK_z:
				// Temp (Remove this later)
				SDL_SetWindowFullscreen(Renderer::GetInstance().GetAppWindow(), 0);
				break;

			default: break;
			}

			break;
		} // KEYBOARD_INPUT END

		if (m_gameState == GameState::MAIN_MENU)
		{
			case SDL_MOUSEMOTION:
			{
				SDL_GetMouseState(&m_mouseX, &m_mouseY);

				if ((m_mouseX >= 1125.0f && m_mouseX <= 1285.0f) && (m_mouseY >= 380.0f && m_mouseY <= 435.0f))
				{
					Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(10.0f, 2.0f, 0.0f));
				}
				else if ((m_mouseX >= 1105.0f && m_mouseX <= 1300.0f) && (m_mouseY >= 485.0f && m_mouseY <= 540.0f))
				{
					Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(9.5f, -2.7f, 0.0f));
				}
				else if ((m_mouseX >= 1150.0f && m_mouseX <= 1265.0f) && (m_mouseY >= 588.0f && m_mouseY <= 639.0f))
				{
					Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(11.0f, -7.0f, 0.0f));
				}

				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				switch (i->button.button)
				{
				case SDL_BUTTON_LEFT:
					 
					// Check if the "Start" button was pressed
					if ((m_mouseX >= 1125.0f && m_mouseX <= 1285.0f) && (m_mouseY >= 380.0f && m_mouseY <= 435.0f))
					{
						// Start game and lock mouse cursor
						m_gameState = GameState::PLAY;
						SDL_SetRelativeMouseMode(SDL_TRUE);
						SDL_CaptureMouse(SDL_TRUE);
					}
					// Check if the "Exit" button was pressed
					else if ((m_mouseX >= 1150.0f && m_mouseX <= 1265.0f) && (m_mouseY >= 588.0f && m_mouseY <= 639.0f))
					{
						m_gameState = GameState::EXIT;
					}

					break;

				default: break;
				}
				break;
			}
		}
		default:
			break;
		}
	}
}