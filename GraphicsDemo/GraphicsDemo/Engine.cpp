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

void Engine::LoadApplicationResources()
{
	ResourceManager::GetInstance().LoadTextureImagesFromFile("Textures/wall.jpg", "wall");
	ResourceManager::GetInstance().LoadTextureImagesFromFile("Textures/mario.png", "mario");
	ResourceManager::GetInstance().LoadTextureImagesFromFile("Textures/level01.png", "level01");
}