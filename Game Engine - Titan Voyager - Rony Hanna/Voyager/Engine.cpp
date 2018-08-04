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

	// Terrain textures
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil01.jpg", "soil"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil02.jpg", "soil2"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil03.jpg", "grass"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil03_NormalMap.jpg", "grassNormalMap"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil04.jpg", "soil4"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/blendMap.png", "blendMap"); assert(bUnitTest);

	std::vector<char*> skyboxFaces;
	skyboxFaces.push_back("res/Textures/Skyboxes/TitanMoon/right.png");
	skyboxFaces.push_back("res/Textures/Skyboxes/TitanMoon/left.png");
	skyboxFaces.push_back("res/Textures/Skyboxes/TitanMoon/top.png");
	skyboxFaces.push_back("res/Textures/Skyboxes/TitanMoon/bottom.png");
	skyboxFaces.push_back("res/Textures/Skyboxes/TitanMoon/front.png");
	skyboxFaces.push_back("res/Textures/Skyboxes/TitanMoon/back.png");

	// Review this (make into a loop)
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(0), "skybox");  assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(1), "skybox1"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(2), "skybox2"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(3), "skybox3"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(4), "skybox4"); assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(5), "skybox5"); assert(bUnitTest);

	ResourceManager::GetInstance().SetSkyboxTextureIDs();
}