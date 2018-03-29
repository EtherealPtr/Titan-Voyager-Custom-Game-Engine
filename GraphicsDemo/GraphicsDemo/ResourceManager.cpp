#include "ResourceManager.h"
#include <iostream>
#include "Dependencies/glew/include/GL/glew.h"

ResourceManager::~ResourceManager()
{
	for (std::vector<int*>::iterator iter = m_w.begin(); iter != m_w.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}

	for (std::vector<int*>::iterator iter = m_h.begin(); iter != m_h.end(); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}

	// Free all images
	for (std::vector<unsigned char*>::iterator iter = m_pImages.begin(); iter != m_pImages.end(); ++iter)
	{
		SOIL_free_image_data(*iter);
	}
}

unsigned char* ResourceManager::GetTexture(char* id)
{
	for (std::map<char*, unsigned char*>::iterator it = m_resourceId.begin(); it != m_resourceId.end(); ++it)
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

int& ResourceManager::GetImageWidth(char* id)
{
	for (std::map<char*, int*>::iterator it = m_widths.begin(); it != m_widths.end(); ++it)
	{
		// Check if the id corresponds to an image id, if so return the corresponding width of the img
		if (id == it->first)
		{
			return *it->second;
		}
	}
}

int& ResourceManager::GetImageHeight(char* id)
{
	for (std::map<char*, int*>::iterator it = m_heights.begin(); it != m_heights.end(); ++it)
	{
		// Check if the id corresponds to an image id, if so return the corresponding height of the img
		if (id == it->first)
		{
			return *it->second;
		}
	}
}

void ResourceManager::LoadTextureImagesFromFile(char* filename, char* id)
{
	width = new int;
	height = new int;

	// Push image (unsigned char*) into vector 
	m_pImages.push_back(SOIL_load_image(filename, width, height, 0, SOIL_LOAD_RGBA));

	if (m_pImages.back() == nullptr)
		std::cerr << "TEXTURE LOAD ERROR: Error occured while loading texture image.\n";

	m_w.push_back(width);
	m_h.push_back(height);

	// Store the pushed image into the map
	m_resourceId[id] = m_pImages.back();
	m_widths[id] = m_w.back();
	m_heights[id] = m_h.back();
}