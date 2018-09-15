#pragma once
#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include "Dependencies\soil\include\SOIL.h"
#include <vector>
#include <string>
#include <map>

class ResourceManager
{
public:
	~ResourceManager();

	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance; 
	}

	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	unsigned char* GetTexture(char* id);
	bool LoadTextureImagesFromFile(char* filename, char* id);
	void LoadAudioFiles();
	void ReleaseAudioFiles();

	void SetSkyboxTextureIDs();
	std::vector<char*> GetSkyboxTextureIds();
	void GetImageDimension(char* id, std::vector<int>& result);

private:
	ResourceManager() {};

	std::map<char*, unsigned char*> m_resourceId;
	std::map<char*, int*> m_heights;
	std::map<char*, int*> m_widths;

	std::vector<unsigned char*> m_pImages;
	std::vector<int*> m_w, m_h;
	std::vector<char*> skyboxTexturesIds;
	int* width, *height;
};

#endif // !__RESOURCEMANAGER_H__