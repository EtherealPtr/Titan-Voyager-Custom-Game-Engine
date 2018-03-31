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
// Description: Function that loads resources and determine whether the loaded items are fit for use
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
}