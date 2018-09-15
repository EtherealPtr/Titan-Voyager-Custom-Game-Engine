#include "ResourceManager.h"
#include "Dependencies/glew/include/GL/glew.h"
#include "Audio.h"
#include <iostream>

ResourceManager::~ResourceManager()
{
	// Deallocate memory
	for (auto iter = m_w.begin(); iter != m_w.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}

	for (auto iter = m_h.begin(); iter != m_h.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}

	// Free all images
	for (auto iter = m_pImages.begin(); iter != m_pImages.end(); ++iter)
	{
		SOIL_free_image_data(*iter);
	}
}

unsigned char* ResourceManager::GetTexture(char* id)
{
	for (auto it = m_resourceId.begin(); it != m_resourceId.end(); ++it)
	{
		// Check if the id corresponds to an image id, if so return the corresponding image
		if (id == it->first)
		{
			return it->second;
		}
	}

	std::cerr << "ERROR: Unable to retrieve texture image file.\n";
	return nullptr;
}

void ResourceManager::GetImageDimension(char* id, std::vector<int>& result)
{
	for (auto it = m_widths.begin(); it != m_widths.end(); ++it)
	{
		// Check if the id corresponds to an image id, if so return the corresponding height of the image
		if (id == it->first)
		{
			int w = *it->second;
			int h = *m_heights[id];

			result.push_back(w);
			result.push_back(h);
			
			break;
		}
	}
}

bool ResourceManager::LoadTextureImagesFromFile(char* filename, char* id)
{
	width = new int;
	height = new int;

	// Push image (unsigned char*) into vector 
	m_pImages.push_back(SOIL_load_image(filename, width, height, 0, SOIL_LOAD_RGBA));

	if (m_pImages.back() == nullptr)
	{
		std::cerr << "Loading failed: An error occurred while loading texture file.\n";
		return false;
	}

	m_w.push_back(width);
	m_h.push_back(height);

	// Store the pushed image into the map
	m_resourceId[id] = m_pImages.back();
	m_widths[id] = m_w.back();
	m_heights[id] = m_h.back();

	return true;
}

void ResourceManager::LoadAudioFiles()
{
	Audio::GetInstance().LoadAudioFile("res/Audio/ButtonHovered.wav", "ButtonHovered");
	Audio::GetInstance().LoadAudioFile("res/Audio/ButtonClicked.wav", "ButtonClicked");
	Audio::GetInstance().LoadAudioFile("res/Audio/AR_Fired.wav", "AR_Fire");
	Audio::GetInstance().LoadAudioFile("res/Audio/FlashLightOn.wav", "FlashOn");
	Audio::GetInstance().LoadAudioFile("res/Audio/FlashLightOff.wav", "FlashOff");
	Audio::GetInstance().LoadAudioFile("res/Audio/EnemyHit.wav", "EnemyHit");
	Audio::GetInstance().LoadAudioFile("res/Audio/Hurt.wav", "PlayerHit");
	Audio::GetInstance().LoadAudioFile("res/Audio/Thunder.wav", "ThunderStorm");
	Audio::GetInstance().LoadAudioFile("res/Audio/FootstepsWalk.wav", "WalkFootsteps", true, 0);
	Audio::GetInstance().LoadAudioFile("res/Audio/FootstepsRun.wav", "RunFootsteps", true, 1);
	Audio::GetInstance().LoadAudioFile("res/Audio/EnemyDead.wav", "EnemyDead");
	Audio::GetInstance().LoadAudioFile("res/Audio/EnemyDead2.wav", "EnemyDead2");
	Audio::GetInstance().LoadAudioFile("res/Audio/MainMenu.mp3", "MainMenu", true, 2); 
	Audio::GetInstance().LoadAudioFile("res/Audio/InGame.mp3", "InGame", true, 3);
	Audio::GetInstance().LoadAudioFile("res/Audio/Reloading.wav", "Reload", true, 4); 
}

void ResourceManager::ReleaseAudioFiles()
{
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("ButtonHovered")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("ButtonClicked")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("AR_Fire")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("FlashOn")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("FlashOff")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("EnemyHit")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("PlayerHit")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("ThunderStorm")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("WalkFootsteps")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("RunFootsteps")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("EnemyDead")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("EnemyDead2")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("Reload")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("MainMenu")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find("InGame")->second);
}

void ResourceManager::SetSkyboxTextureIDs()
{
	// Initializer list to store skybox IDs
	skyboxTexturesIds = { "skybox" , "skybox1", "skybox2", "skybox3", "skybox4", "skybox5" };
}

std::vector<char*> ResourceManager::GetSkyboxTextureIds()
{
	return skyboxTexturesIds;
}