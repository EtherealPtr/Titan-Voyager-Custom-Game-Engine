#pragma once
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <vector>
#include "Camera.h"
#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"
#include "Dependencies/glm-0.9.9-a2/glm/glm.hpp"
#include "Dependencies/glm-0.9.9-a2/glm/gtx/transform.hpp"
#include "PerlinNoise.h"
#include "Shader.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Terrain
{
public:
	Terrain();
	~Terrain();

	void LoadHeightmapImage(const char* FileName);
	float GetHeightOfTerrain(float _X, float _Z);
	float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	void InitTerrain(char* vs, char* fs);
	void CreateTerrainWithPerlinNoise();
	glm::vec3 CalculateNormal(unsigned int x, unsigned int z);
	void SetFog(bool fogState) { m_fog = fogState; }

	void Draw(Camera& cam, DirectionalLight* directionLight, PointLight* lamp, SpotLight* spotlight);

private:
	Shader m_terrainShader;
	Texture m_terrainTexture;
	GLuint m_terrainTextures[5];
	glm::mat4 m_model;

private:
	enum { VERTEX_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, ELEMENT_BUFFER, TOTAL_BUFFERS };

	GLuint m_VAO;
	GLuint m_VBO[TOTAL_BUFFERS];
	SDL_Surface* m_pImage;

	float m_cellSpacing, m_fTerrainHeight;
	float m_terrainLength;
	float m_terrainWidth;
	float m_terrainXPos;
	float m_terrainZPos;
	bool m_fog;

	std::vector<std::vector<float> > m_vHeights;
	std::vector<unsigned int> m_indices;

private:
	std::uint32_t seed;
	PerlinNoise noise;
};

#endif // !__TERRAIN_H__