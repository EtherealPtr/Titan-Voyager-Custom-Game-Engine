#include "Engine.h"
#include <cassert>

Engine::Engine()
{}

Engine::~Engine()
{}

void Engine::Run()
{
	LoadApplicationResources();
	Renderer::GetInstance().Run(); 
	m_game.Run();
}

// -------------------
// Author: Rony Hanna
// Description: Function that loads external resources and uses unit testing to ensure that items were loaded 
// -------------------
void Engine::LoadApplicationResources()
{
	bool bUnitTest;

	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/cubeTex.png", "cubeTex"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/cubeTex_NormalMap.png", "cubeTexNormalMap"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/saturnTex.jpg", "saturn"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/saturn_rings.png", "saturnRings"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/crossHair.png", "crossHair"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/enemy01.jpg", "enemySphere"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/muzzleFlash.png", "muzzleFlash"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/Ammo.png", "ammo"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/HealthIcon.png", "health"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/SniperScope.png", "sniperScope"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/RedOrb.png", "redOrb"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/Camouflage.jpg", "clothTex"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/shockwave.png", "shockwave"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/drone.jpg", "drone"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/menu.png", "mainMenu"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/Indicator.png", "indicator"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/AboutBackground.png", "aboutMenu"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/PlayerDeathScreen.png", "playerDead"); assert(bUnitTest); 
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/VictoryScreen.png", "victorious"); assert(bUnitTest);

	// Terrain textures
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil01.jpg", "soil"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil02.jpg", "soil2"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil03.jpg", "grass"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil03_NormalMap.jpg", "grassNormalMap"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil04.jpg", "soil4"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/blendMap.png", "blendMap"); assert(bUnitTest);

	std::vector<char*> skyboxFaces = 
	{
		"res/Textures/Skyboxes/TitanMoon/right.png", "res/Textures/Skyboxes/TitanMoon/left.png", 
		"res/Textures/Skyboxes/TitanMoon/top.png", "res/Textures/Skyboxes/TitanMoon/bottom.png", 
		"res/Textures/Skyboxes/TitanMoon/front.png", "res/Textures/Skyboxes/TitanMoon/back.png"
	};

	std::vector<char*> skyboxIDs = { "skybox", "skybox1", "skybox2", "skybox3", "skybox4", "skybox5" };

	for (auto i = 0; i < 6; ++i) 
	{
		bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(i), skyboxIDs.at(i)); 
		assert(bUnitTest);
	}
	 
	ResourceManager::GetInstance().SetSkyboxTextureIDs();
}