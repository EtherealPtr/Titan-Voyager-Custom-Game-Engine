#include "Game.h"
#include "Utils.h"
#include "ResourceManager.h"

std::vector<SDL_Event>& GetFrameEvents()
{
	static std::vector<SDL_Event> frame_events;
	return frame_events;
}

Game::Game() :
	m_deltaTime(0.0f),
	m_gameState(GameState::MAIN_MENU),
	m_dataTransmitTimer(0.0f), 
	m_gameStateTimer(0.0f),
	m_enemyCount(1)
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

	// Deallocate the asteroid's model matrices
	delete m_asteroid.meshes[0].GetModelMatIns();

	// Release all audio files
	ResourceManager::GetInstance().ReleaseAudioFiles();
}

void Game::Run()
{
	InitMeshes();
	InitLights();
	InitAudio();
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
	Renderer::GetInstance().InitMesh(QUAD, "aboutMenu", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f));
	Renderer::GetInstance().InitMesh(QUAD, "playerDead", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(17.0f, 17.0f, 17.0f));
	Renderer::GetInstance().InitMesh(QUAD, "victorious", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(17.0f, 17.0f, 17.0f));

	// Enemy ID registeration [100, 130] inclusively 
	id = 100;
	for (unsigned int i = 0; i < 30; ++i)
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
	dataTransmissionText.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	dataTransmissionText.SetSpacing(0.7f);
	dataTransmissionText.SetPosition(glm::vec2(600.0f, 40.0f));
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

void Game::InitAudio()
{
	Audio::GetInstance().Init(); 
	ResourceManager::GetInstance().LoadAudioFiles();
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
		else if (m_gameState == GameState::ABOUT)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			UpdateMenu();
			Renderer::GetInstance().GetComponent(INDICATOR).Draw(m_cameraHUD);
			Renderer::GetInstance().GetComponent(ABOUT).Draw(m_cameraHUD);
		}
		else if (m_gameState != GameState::EXIT)
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
	if (Player::GetInstance().IsPlayerAiming())
	{
		// Render sniper scope
		Renderer::GetInstance().GetComponent(SNIPER_SCOPE).Draw(m_cameraHUD);

		// Zoom in and slow player speed and mouse sensitivity
		if (m_camera.GetCameraFOV() != 30.0f)
		{
			m_camera.InitCameraPerspective(30.0f, 1440.0f / 900.0f, 0.1f, 5000.0f);
			m_camera.SetCameraSpeed(7.0f);
			m_camera.SetCameraSensitivity(4.0f);
		}
	}
	else
	{
		// Render FPS crosshair if player is not using sniper rifle
		if (!Player::GetInstance().IsUsingSniper())
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

	m_terrain.SetFog(m_atmosphere.GetDayTime() <= 0.3f ? false : true);
	m_terrain.Draw(m_camera, &m_dirLight, &m_pointLight, Player::GetInstance().GetSpotLight());

	Renderer::GetInstance().GetComponent(SKYBOX).GetTransformComponent().GetRot().y += 0.5f * m_deltaTime;
	Renderer::GetInstance().GetComponent(SKYBOX).Draw(m_camera);

	// Draw enemy units
	int enemyId = 100;
	for (unsigned int i = 0; i < m_enemyCount; ++i)
	{
		// Check if this enemy unit can respawn (if the data transfer is at 100, then this enemy cannot respawn anymore)
		m_enemies.at(i)->SetRespawnStatus(m_dataTransmitTimer < 100 ? true : false);
		m_enemies.at(i)->Draw(enemyId, ENEMY_DRONE);
		++enemyId;
	}

	// Draw enemy shockwave if smart drones have exploded
	for (unsigned int i = 0; i < m_enemyCount; ++i)
	{
		m_enemies.at(i)->DrawShockwave(ENEMY_BLAST);
	}

	// Text updates
	// [0] : Ammo, [1] : Health, [2] : Data transmission percentage
	m_texts[0].SetText(std::to_string(Player::GetInstance().GetCurrWeapon().GetAmmoCount()));
	m_texts[0].Render();

	m_texts[1].SetText(std::to_string(Player::GetInstance().GetHealth()));
	m_texts[1].Render();

	if (m_dataTransmitTimer < 100)
	{
		m_texts[2].SetText("Data transfer: " + std::to_string((int)m_dataTransmitTimer) + "%");
	}
	else
	{
		m_texts[2].SetSpacing(0.7f);
		m_texts[2].SetText("Data transferred! Defeat all remaining foes");
	}

	m_texts[2].Render();
}

void Game::UpdateGame()
{
	m_camera.UpdateLookAt();
	Player::GetInstance().Update(m_camera, m_terrain, m_deltaTime, GetFrameEvents());

	if (Player::GetInstance().IsPlayerDead())
	{
		m_gameStateTimer += 1.0f * m_deltaTime;
		Renderer::GetInstance().GetComponent(PLAYER_DEATH_SCREEN).Draw(m_cameraHUD);
		
		if (m_gameStateTimer > 5.0f)
		{
			RestartGame();
			m_gameStateTimer = 0.0f;
		}
	}
	else if (Player::GetInstance().GetHealth() <= 10)
	{
		Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().ActivateProgram();
		Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().SetBool("grayScaleEffect", true);
	}
	else
	{
		Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().ActivateProgram();
		Renderer::GetInstance().GetComponent(POSTPROCESSING_QUAD).GetShaderComponent().SetBool("grayScaleEffect", false);
	}

	if (m_dataTransmitTimer < 100)
	{
		// Increase total number of enemies over time
		m_enemySpawnTimer += 1.0f * m_deltaTime;

		if (m_enemySpawnTimer >= 16.0f)
		{
			m_enemySpawnTimer = 0.0f;

			if (m_enemyCount < 30)
			{
				++m_enemyCount;
			}
		}
	}

	// Update enemy units
	for (unsigned int i = 0; i < m_enemyCount; ++i)
	{
		m_enemies.at(i)->Update(m_terrain, m_camera, m_deltaTime);
	}

	// Update physics component
	Physics::GetInstance().Update(m_camera, m_deltaTime, GetFrameEvents(), m_enemies);

	// Update mountain rocks' fog effect 
	if (m_atmosphere.GetDayTime() >= 0.3f && m_atmosphere.GetDayTime() <= 0.31f)
	{
		for (auto i : m_mountainRocks)
		{
			i.GetShaderProgram().ActivateProgram();
			i.GetShaderProgram().SetBool("nightFog", true);
		}
	}
	else if (m_atmosphere.GetDayTime() < 0.3f && m_atmosphere.GetDayTime() >= 0.29f)
	{
		for (auto i : m_mountainRocks)
		{
			i.GetShaderProgram().ActivateProgram();
			i.GetShaderProgram().SetBool("nightFog", false);
		}
	}

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

	// Update audio
	Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find("InGame")->second, 3, 0.03f);
	Audio::GetInstance().Update();

	// Update data transmitter 
	m_dataTransmitTimer += 0.59f * m_deltaTime;

	// Check if win condition is met (if the data was sent in full and all enemies have been neutralized)
	if (m_dataTransmitTimer >= 100)
	{
		bool m_endGame = false;

		// Loop through the enemies 
		for (unsigned int i = 0; i < m_enemyCount; ++i)
		{
			// Check if the enemy is dead and cannot be respawned
			if (m_enemies.at(i)->GetDeath() && !m_enemies.at(i)->GetRespawnStatus())
			{
				m_endGame = true;
			}
			else
			{
				m_endGame = false;
				break;
			}
		}

		if (m_endGame)
		{
			Renderer::GetInstance().GetComponent(PLAYER_VICTORY_SCREEN).Draw(m_cameraHUD);
			m_gameStateTimer += 1.15f * m_deltaTime;

			if (m_gameStateTimer >= 5.0f)
			{
				FreeMouseCursor();
				m_gameStateTimer = 0.0f;
				Audio::GetInstance().StopSound(3);
				m_gameState = GameState::MAIN_MENU;
			}
		}
	}

	GetFrameEvents().clear();
}

void Game::UpdateMenu()
{
	GetFrameEvents().clear();
	Audio::GetInstance().PlaySoundOnCustomChannel(Audio::GetInstance().GetSoundsMap().find("MainMenu")->second, 2);
}

void Game::RestartGame()
{
	// Restart in-game music
	Audio::GetInstance().StopSound(3);

	m_atmosphere.Restart();

	// Respawn the player (reset position, ammo, health, flashlight)
	Player::GetInstance().Respawn(m_camera);
	
	// Restart existing enemy units
	for (unsigned int i = 0; i < m_enemyCount; ++i)
	{
		m_enemies.at(i)->SetDeath(true);
		m_enemies.at(i)->SetRespawnStatus(true);
	}

	m_dataTransmitTimer = 0.0f;
	m_enemySpawnTimer = 0.0f;
	m_enemyCount = 1;

	// Ensure main menu music's channel is stopped
	Audio::GetInstance().StopSound(2);
}

void Game::FreezeMouseCursor()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);
}

void Game::FreeMouseCursor()
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_CaptureMouse(SDL_FALSE);
}

inline void Game::HideIndicator()
{
	Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(0.0f, 21.0f, 0.0f));
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

			case SDLK_h:
				static bool bFullscreen;

				if (!bFullscreen)
				{
					SDL_SetWindowFullscreen(Renderer::GetInstance().GetAppWindow(), SDL_WINDOW_FULLSCREEN);
					bFullscreen = !bFullscreen;
				}
				else
				{
					SDL_SetWindowFullscreen(Renderer::GetInstance().GetAppWindow(), 0);
					bFullscreen = !bFullscreen;
				}

				break;

			default: break;
			}

			break;
		}

		case SDL_KEYUP:
		{
			switch (i->key.keysym.sym)
			{
			//case SDLK_x: // DEBUG PURPOSES
			//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//	break;

			default: break;
			}

			break;
		} // KEYBOARD_INPUT END

		if (m_gameState == GameState::MAIN_MENU || m_gameState == GameState::ABOUT)
		{
			case SDL_MOUSEMOTION:
			{
				SDL_GetMouseState(&m_mouseX, &m_mouseY);

				// Check if the Main Menu is active
				if (m_gameState == GameState::MAIN_MENU)
				{
					// Check if the mouse is hovering over the START button
					if ((m_mouseX >= 1125.0f && m_mouseX <= 1285.0f) && (m_mouseY >= 380.0f && m_mouseY <= 435.0f))
					{
						if (Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().GetPos() != glm::vec3(10.0f, 2.0f, 0.0f))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonHovered")->second);
							Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(10.0f, 2.0f, 0.0f));
						}
					}
					// Check if the mouse is hovering over the ABOUT button
					else if ((m_mouseX >= 1105.0f && m_mouseX <= 1300.0f) && (m_mouseY >= 485.0f && m_mouseY <= 540.0f))
					{
						if (Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().GetPos() != glm::vec3(9.5f, -2.7f, 0.0f))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonHovered")->second);
							Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(9.5f, -2.7f, 0.0f));
						}
					}
					// Check if the mouse is hovering over the EXIT button
					else if ((m_mouseX >= 1150.0f && m_mouseX <= 1265.0f) && (m_mouseY >= 588.0f && m_mouseY <= 639.0f))
					{
						if (Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().GetPos() != glm::vec3(11.0f, -7.0f, 0.0f))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonHovered")->second);
							Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(11.0f, -7.0f, 0.0f));
						}
					}
					else
					{
						HideIndicator();
					}
				}
				// Otherwise, it is the about menu
				else
				{
					if ((m_mouseX >= 48 && m_mouseX <= 144) && (m_mouseY >= 800 && m_mouseY <= 867))
					{
						if (Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().GetPos() != glm::vec3(-19.0f, -17.0f, 0.0f))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonHovered")->second);
							Renderer::GetInstance().GetComponent(INDICATOR).GetTransformComponent().SetPos(glm::vec3(-19.0f, -17.0f, 0.0f));
						}
					}
					else
					{
						HideIndicator();
					}
				}

				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				switch (i->button.button)
				{
				case SDL_BUTTON_LEFT:
					// Check if the Main Menu is active
					if (m_gameState == GameState::MAIN_MENU)
					{
						// Check if the START button was pressed
						if ((m_mouseX >= 1125.0f && m_mouseX <= 1285.0f) && (m_mouseY >= 380.0f && m_mouseY <= 435.0f))
						{
							// Start game and lock mouse cursor
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonClicked")->second);
							m_gameState = GameState::PLAY;
							RestartGame();
							FreezeMouseCursor();
						}
						// Check if the ABOUT button was pressed
						else if ((m_mouseX >= 1105.0f && m_mouseX <= 1300.0f) && (m_mouseY >= 485.0f && m_mouseY <= 540.0f))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonClicked")->second);
							m_gameState = GameState::ABOUT;
							HideIndicator();
						}
						// Check if the EXIT button was pressed
						else if ((m_mouseX >= 1150.0f && m_mouseX <= 1265.0f) && (m_mouseY >= 588.0f && m_mouseY <= 639.0f))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonClicked")->second);
							m_gameState = GameState::EXIT;
						}
					}
					// Otherwise, it is the about menu
					else
					{
						// Check if the back button was pressed
						if ((m_mouseX >= 48 && m_mouseX <= 144) && (m_mouseY >= 800 && m_mouseY <= 867))
						{
							Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("ButtonClicked")->second);
							m_gameState = GameState::MAIN_MENU;
							HideIndicator();
						}
					}

					break;

				default: break;
				}
				break;
			}
		}

		default: break;
		}
	}
}