#include "Game.h"

std::vector<SDL_Event>& GetFrameEvents()
{
	static std::vector<SDL_Event> frame_events;
	return frame_events;
}

Game::Game() :
	m_deltaTime(0.0f),
	m_gameState(GameState::PLAY),
	m_spaceScene(false)
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_camera.InitCameraPerspective(80.0f, 1440.0f / 900.0f, 0.1f, 5000.0f);
	m_cameraHUD.InitCameraOrthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
}

Game::~Game()
{}

void Game::Run()  
{      
	InitMeshes();
	//InitParticleSystem();
	InitLights();
	InitDebugger();
	InitText();
	GameLoop();
}

void Game::InitMeshes()
{
	int id = 0;
	std::vector<char*> defShader{ "res/Shaders/DefaultVertexShader.vs", "res/Shaders/DefaultFragmentShader.fs" };
	std::vector<char*> skyboxShader{ "res/Shaders/SkyboxVertexShader.vs", "res/Shaders/SkyboxFragmentShader.fs" };
	std::vector<char*> normalMappingShader{ "res/Shaders/NormalMapping.vs", "res/Shaders/NormalMapping.fs" };
	std::vector<char*> hudShader{ "res/Shaders/HUD.vs", "res/Shaders/HUD.fs" };
	std::vector<char*> muzzleFlashShader{ "res/Shaders/Muzzle Flash Shader/VertexShaderMuzzleFlash.vs", "res/Shaders/Muzzle Flash Shader/FragmentShaderMuzzleFlash.fs" };

	Renderer::GetInstance().InitMesh(QUAD, "saturnRings", ++id, defShader, glm::vec3(200.0f, 160.0f, -500.0f), glm::vec3(-65.0f, 0.0f, 0.0f), glm::vec3(330.0f, 330.0f, 330.0f));
	Renderer::GetInstance().InitMesh(CUBE, "cubeTex", ++id, normalMappingShader, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f), "cubeTexNormalMap");
	Renderer::GetInstance().InitMesh(CUBE, "skybox", ++id, skyboxShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3000.0f, 3000.0f, 3000.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "saturn", ++id, defShader, glm::vec3(200.0f, 150.0f, -500.0f), glm::vec3(25.0f, 90.0f, 0.0f), glm::vec3(55.0f, 55.0f, 55.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "mars", ++id, defShader, glm::vec3(-70.0f, 50.0f, -70.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(6.36f, 6.36f, 6.36f));
	Renderer::GetInstance().InitMesh(SPHERE, "mercury", ++id, defShader, glm::vec3(50.0f, 45.0f, -60.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(4.56f, 4.56f, 4.56f));
	Renderer::GetInstance().InitMesh(SPHERE, "neptune", ++id, defShader, glm::vec3(-200.0f, -70.0f, -180.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(40.0f, 40.0f, 40.0f));
	Renderer::GetInstance().InitMesh(SPHERE, "earth", ++id, defShader, glm::vec3(0.0f, 0.0f, -60.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(12.0f, 12.0f, 12.0f));
	Renderer::GetInstance().InitMesh(QUAD, "crossHair", ++id, hudShader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Renderer::GetInstance().InitMesh(QUAD, "ammo", ++id, hudShader, glm::vec3(-9.0f, -9.0f, 0.0f), glm::vec3(0.0f, 0.0f, 105.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	Renderer::GetInstance().InitMesh(QUAD, "health", ++id, hudShader, glm::vec3(-6.0f, -9.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(0.95f, 1.2f, 1.0f));
	Renderer::GetInstance().InitMesh(QUAD, "muzzleFlash", ++id, muzzleFlashShader, glm::vec3(2.0f, -2.5f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	m_terrain.InitTerrain("res/Shaders/TerrainVertexShader.vs", "res/Shaders/TerrainFragmentShader.fs");
	m_terrain.CreateTerrainWithPerlinNoise();

	Enemy* enemy01 = new Enemy(m_camera);
	m_enemies.push_back(enemy01);
	m_enemies.at(0)->InitMesh();

	m_weapon.Init("res/Models3D/Rifle/M24_R.obj", m_camera, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs", false);
	//m_aircraft.Init("res/Models3D/Walkyrie/object.obj", m_camera, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs", false);
	//m_asteroid.Init("res/Models3D/Rock/rock.obj", m_camera, "res/Shaders/InstancingVert.vs", "res/Shaders/InstancingFrag.fs", true);
}

void Game::InitLights()
{
	m_dirLight.Configure(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f));
	m_dirLight.SetDirection(glm::vec3(0.2f, 1.0f, 0.5f));
	m_dirLight.SetColour(glm::vec3(0.97f, 0.88f, 0.70f));

	m_pointLight.Configure(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.014f, 0.0007f);
	m_pointLight.SetPosition(glm::vec3(-39.0f, 50.0f, 44.0f));
	m_pointLight.SetLightColour(glm::vec3(1.0f, 0.0f, 0.0f));

	m_spotlight.Configure(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 22.5f, 25.0f);
}

void Game::InitDebugger()
{
	m_debugger.Init(m_camera);
}

void Game::InitParticleSystem()
{
	//m_particleSystem.Init("res/Shaders/Particle System Shaders/VertexShader.vs", 
	//					  "res/Shaders/Particle System Shaders/GeometryShader.geom", 
	//					  "res/Shaders/Particle System Shaders/FragmentShader.fs", 25); 
}

void Game::InitText()
{
	Text ammoText;
	ammoText.Configure("res/Fonts/Roboto-BoldItalic.ttf");
	ammoText.SetText("35");
	ammoText.SetScale(0.9f);
	ammoText.SetSpacing(0.9f);
	ammoText.SetPosition(glm::vec2(120.0f, 30.0f));
	m_texts.push_back(ammoText);

	Text healthText;
	healthText.Configure("res/Fonts/Roboto-BoldItalic.ttf");
	healthText.SetText("100");
	healthText.SetScale(0.9f);
	healthText.SetSpacing(0.9f);
	healthText.SetPosition(glm::vec2(365.0f, 30.0f));
	m_texts.push_back(healthText);

	//Text redOrbText;
	//redOrbText.Configure("res/Fonts/Roboto-BoldItalic.ttf");
	//redOrbText.SetText("Text ");
	//redOrbText.SetScale(0.5f);
	//redOrbText.SetSpacing(0.7f);
	//redOrbText.SetPosition(glm::vec2(1200.0f, 850.0f));
	//m_texts.push_back(redOrbText);
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
		
		SDL_Event _event;
		while (SDL_PollEvent(&_event) != 0)
		{
			GetFrameEvents().push_back(_event);
		}
 
		ProcessInput(GetFrameEvents());
		Update();

		// Remove this in the future
		if (m_physics.GetDebugRayCastDraw())
		{
			m_debugger.DrawRay(m_physics.GetRay().pos, m_physics.GetRay().dir, m_camera);
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		//m_spotlight.SetPosition(glm::vec3(m_camera.GetCameraPos().x, m_camera.GetCameraPos().y, m_camera.GetCameraPos().z));
		//m_spotlight.SetDirection(glm::vec3(m_camera.GetCameraForward().x, m_camera.GetCameraForward().y, m_camera.GetCameraForward().z));

		Renderer::GetInstance().GetComponent(FPS_CROSSHAIR).Draw(m_cameraHUD);
		Renderer::GetInstance().GetComponent(AMMO).Draw(m_cameraHUD);
		Renderer::GetInstance().GetComponent(HEALTH).Draw(m_cameraHUD);

		if (m_spaceScene)
		{
			Renderer::GetInstance().GetComponent(SPACE_CUBE).Draw(m_camera, glm::vec3(0.0f, 0.0f, 0.0f), true);
			Renderer::GetInstance().GetComponent(SPACE_CUBE).GetTransformComponent().GetRot().y += 6.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(SPACE_CUBE).GetTransformComponent().GetRot().x += 6.0f * m_deltaTime;
				
			// Planets
			Renderer::GetInstance().GetComponent(SATURN_RINGS).Draw(m_camera);
			Renderer::GetInstance().GetComponent(SATURN).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(SATURN).Draw(m_camera);

			Renderer::GetInstance().GetComponent(MARS).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(MARS).Draw(m_camera);

			Renderer::GetInstance().GetComponent(MERCURY).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(MERCURY).Draw(m_camera);

			Renderer::GetInstance().GetComponent(NEPTUNE).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(NEPTUNE).Draw(m_camera);

			Renderer::GetInstance().GetComponent(EARTH).GetTransformComponent().GetRot().y += 2.0f * m_deltaTime;
			Renderer::GetInstance().GetComponent(EARTH).Draw(m_camera);
			
			m_asteroid.DrawInstanced(m_camera);
		}

		glDisable(GL_CULL_FACE);

		m_terrain.Draw(m_camera, &m_dirLight, &m_pointLight, &m_spotlight);
		//m_aircraft.Draw(m_camera, glm::vec3(100.0f, 0.0f, 40.0f), glm::vec3(1.0f), 0.0f, glm::vec3(0.2f, 0.2f, 0.2f));
		
	    //m_particleSystem.Render(m_camera, m_deltaTime, glm::vec3(m_camera.GetCameraPos().x, m_camera.GetCameraPos().y, m_camera.GetCameraPos().z));

		Renderer::GetInstance().GetComponent(SKYBOX).Draw(m_camera);

		// Text updates
		// [0] : Ammo [1] : Health
		m_texts[0].SetText(std::to_string(m_player.GetAmmoCount()));
		m_texts[0].Render();
		
		m_texts[1].SetText(std::to_string(m_player.GetHealth()));
		m_texts[1].Render();

		SDL_GL_SwapWindow(Renderer::GetInstance().GetAppWindow());
		SDL_Delay(1);

		GetFrameEvents().clear();
	}
}

void Game::Update()
{
	m_camera.UpdateLookAt();

	m_player.Update(m_weapon, m_camera, m_terrain, m_deltaTime, GetFrameEvents());

	// Update enemies
	for (auto i = m_enemies.begin(); i != m_enemies.end(); ++i)
	{
		(*i)->Seek(m_camera, m_deltaTime);
		(*i)->Update(m_terrain, m_camera);
		(*i)->Draw();
	}

	m_physics.Update(m_camera, m_deltaTime, GetFrameEvents(), m_enemies);
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
				Renderer::GetInstance().GetComponent(SKYBOX).GetTextureComponent().GenerateSkybox(6, 12);
				m_spaceScene = false;
				break;

			default: break;
			}

			break;
		}
		// KEYBOARD_INPUT END

		default:
			break;
		}
	}
}