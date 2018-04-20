#include "Engine.h"
#include <cassert>

Engine::Engine()
{
}

Engine::~Engine()
{
}

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

	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/wall.jpg", "wall");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/mario.png", "mario");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/cubeTex.png", "cubeTex");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/saturnTex.jpg", "saturn");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/mars.jpg", "mars");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/mercury.jpg", "mercury");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/neptune.jpg", "neptune");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/earth.jpg", "earth");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/saturn_rings.png", "saturnRings");
	assert(bUnitTest);

	std::vector<char*> skyboxFaces;
	skyboxFaces.push_back("res/Textures/right.jpg");
	skyboxFaces.push_back("res/Textures/left.jpg");
	skyboxFaces.push_back("res/Textures/top.jpg");
	skyboxFaces.push_back("res/Textures/bottom.jpg");
	skyboxFaces.push_back("res/Textures/front.jpg");
	skyboxFaces.push_back("res/Textures/back.jpg");

	// Terrain textures
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil01.jpg", "soil");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil02.jpg", "soil2");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil03.jpg", "soil3");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/soil04.jpg", "soil4");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile("res/Textures/blendMap.png", "blendMap");
	assert(bUnitTest);

	// Review this (make into a loop)
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(0), "skybox");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(1), "skybox1");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(2), "skybox2");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(3), "skybox3");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(4), "skybox4");
	assert(bUnitTest);
	bUnitTest = ResourceManager::GetInstance().LoadTextureImagesFromFile(skyboxFaces.at(5), "skybox5");
	assert(bUnitTest);

	ResourceManager::GetInstance().SetSkyboxTextureIDs();
}