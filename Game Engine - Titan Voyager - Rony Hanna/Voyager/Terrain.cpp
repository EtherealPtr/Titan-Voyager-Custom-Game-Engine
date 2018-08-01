#include "Terrain.h"
#include <fstream>
#include "Dependencies\soil\include\SOIL.h"
#include <cstdlib>
#include <ctime>
#include "ResourceManager.h"
#include <iostream>

// -------------------
// Author: Rony Hanna
// Description: Constructor that initializes terrain components 
// -------------------
Terrain::Terrain() :
	m_fTerrainHeight(70.0f), m_cellSpacing(3.0f),
	m_terrainLength(256), m_terrainWidth(256),
	m_model(1.0f),
	m_terrainXPos(0.0f),
	m_terrainZPos(0.0f)
{
	m_model = glm::translate(glm::vec3(m_terrainXPos, 0.0f, m_terrainZPos));
}

// -------------------
// Author: Rony Hanna
// Description: Destructor that deallocates allocated memory on the heap 
// -------------------
Terrain::~Terrain()
{}

// -------------------
// Author: Rony Hanna
// Description: Function that creates the terrain's shader program and initializes the terrain's textures
// -------------------
void Terrain::InitTerrain(char* vs, char* fs)
{
	m_terrainShader.CreateProgram(vs, fs);
	std::vector<char*> images{ "soil", "soil2", "grass", "soil4", "blendMap", "grassNormalMap" };
	m_terrainTexture.GenerateMultipleTextures(images);
}

// -------------------
// Author: Rony Hanna
// Description: Function that reads a heightmap (a 2D grid of values) and sends the data to the GPU
// -------------------
void Terrain::LoadHeightmapImage(const char* FileName)
{
	m_pImage = SDL_LoadBMP(FileName);
	Uint32 Pixel = 0;

	if (m_pImage == nullptr)
	{
		std::cerr << "ERROR: Unable to load heightmap.\n";
		return;
	}

	std::vector<float> tmp;
	for (int i = 0; i < m_pImage->h; ++i)
	{
		for (int j = 0; j < m_pImage->w; ++j)
		{
			Pixel = ((Uint32*)m_pImage->pixels)[i * m_pImage->pitch / 4 + j];
			unsigned char r, g, b;
			SDL_GetRGB(Pixel, m_pImage->format, &r, &g, &b);
			tmp.push_back((float)r / 255.0f);
		}

		m_vHeights.push_back(tmp);
		tmp.clear();
	}

	SDL_FreeSurface(m_pImage);

	float loadingCount = 0.0f;
	int iTotalLoadingCycles = m_pImage->h * m_pImage->w;

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Textures;
	std::vector<glm::vec3> Normals;

	for (unsigned int i = 0; i < m_vHeights.size(); ++i)
	{
		for (unsigned int j = 0; j < m_vHeights[0].size(); ++j)
		{
			if (m_vHeights[i][j] <= 0.0)
			{
				continue;
			}

			Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight, j * m_cellSpacing));
			Textures.push_back(glm::vec2(i * 1.0f / m_vHeights.size(), j * 1.0f / m_vHeights[0].size()));
			Normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	for (unsigned int i = 0; i < m_vHeights.size() - 1; ++i)
	{
		for (unsigned int j = 0; j < m_vHeights.size() - 1; ++j)
		{
			++loadingCount;
			m_indices.push_back((i * m_vHeights[0].size()) + j);
			m_indices.push_back((i * m_vHeights[0].size()) + j + 1);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j);

			m_indices.push_back((i * m_vHeights[0].size()) + j + 1);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j + 1);
		}

		std::cout << "Loading Terrain: " << loadingCount << " / " << iTotalLoadingCycles << " \r";
	}

	std::cout << "Terrain loading operation completed \n";

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glGenBuffers(1, &m_VBO[TEXTURE_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURE_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Textures.size() * sizeof(glm::vec2), &Textures[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glGenBuffers(1, &m_VBO[NORMAL_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), &Normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glGenBuffers(1, &m_VBO[ELEMENT_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[ELEMENT_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "Terrain loaded successfully!\n";
}

// -------------------
// Author: Rony Hanna
// Description: Function that creates a heightmap (procedurally) using the Perlin Noise algorithm
// -------------------
void Terrain::CreateTerrainWithPerlinNoise()
{
	// Set the seed of the noise so that it is different every time
	noise.SetSeed(static_cast<unsigned int>(time(0)));
	double frequency = 4.2;
	int octaves = 5;

	const double fx = 256.0 / frequency;
	const double fy = 256.0 / frequency;

	std::vector<float> tmp;
	for (unsigned int y = 0; y < 256; ++y)
	{
		for (unsigned int x = 0; x < 256; ++x)
		{
			const RGB color(noise.OctaveNoise(x / fx, y / fy, octaves));
			tmp.push_back((float)color.r);
		}

		m_vHeights.push_back(tmp);
		tmp.clear();
	}

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Textures;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> tangents;

	// Calculate vertices
	int terrainHeightOffsetFront = 50;
	int terrainHeightOffsetBack = 0;
	int terrainHeightOffsetLeftSide = 50;
	int terrainHeightOffsetRightSide = 0;

	for (unsigned int i = 0; i < m_vHeights.size(); ++i)
	{
		terrainHeightOffsetFront = 50;
		terrainHeightOffsetBack = 0;
		terrainHeightOffsetLeftSide -= 4;

		if (i > 245)
			terrainHeightOffsetRightSide += 4;

		for (unsigned int j = 0; j < m_vHeights[0].size(); ++j)
		{
			if (i < 12)
			{
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetLeftSide, j * m_cellSpacing));
			}
			else if (i > 245)
			{
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetRightSide, j * m_cellSpacing));
			}
			else if (j < 12)
			{
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetFront, j * m_cellSpacing));
				terrainHeightOffsetFront -= 4;
			}
			else if (j > 245)
			{
				terrainHeightOffsetBack += 4;

				if (terrainHeightOffsetBack > 70)
					terrainHeightOffsetBack = 70;

				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetBack, j * m_cellSpacing));
			}
			else
			{
				terrainHeightOffsetFront = 50;
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight, j * m_cellSpacing));
			}

			//Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight, j * m_cellSpacing));
			Textures.push_back(glm::vec2(i * 1.0f / m_vHeights.size(), j * 1.0f / m_vHeights[0].size()));
			Normals.push_back(CalculateNormal(i, j));
		}
	}

	// Calculate indices
	for (unsigned int i = 0; i < m_vHeights.size() - 1; ++i)
	{
		for (unsigned int j = 0; j < m_vHeights.size() - 1; ++j)
		{
			m_indices.push_back((i * m_vHeights[0].size()) + j);
			m_indices.push_back((i * m_vHeights[0].size()) + j + 1);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j);

			m_indices.push_back((i * m_vHeights[0].size()) + j + 1);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j + 1);
		}
	}

	// Calculate tangents
	for (unsigned int i = 0; i < m_vHeights.size(); ++i)
	{
		for (unsigned int j = 0; j < m_vHeights[0].size(); ++j)
		{
			int vertexIndex = j + i * 256;
			glm::vec3 v1 = Vertices[vertexIndex];

			if (j < 255)
			{
				glm::vec3 v2 = Vertices[vertexIndex + 1];
				glm::vec3 result = v1 - v2;
				result = glm::normalize(result);
				tangents.push_back(result);
			}
			else
			{
				glm::vec3 v2 = Vertices[vertexIndex - 1];
				glm::vec3 result = v1 - v2;
				result = glm::normalize(result);
				tangents.push_back(result);
			}
		}
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(TOTAL_BUFFERS, m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURE_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Textures.size() * sizeof(glm::vec2), &Textures[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TANGENT_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glGenBuffers(1, &m_VBO[ELEMENT_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[ELEMENT_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// -------------------
// Author: Rony Hanna
// Description: Function that calculate the normals for the terrain
// -------------------
glm::vec3 Terrain::CalculateNormal(unsigned int x, unsigned int z)
{
	if (x >= 0 && x < m_terrainWidth - 1 && z >= 0 && z < m_terrainLength - 1)
	{
		float heightL = GetHeightOfTerrain((float)x - 1, (float)z);
		float heightR = GetHeightOfTerrain((float)x + 1, (float)z);
		float heightD = GetHeightOfTerrain((float)x, (float)z - 1);
		float heightU = GetHeightOfTerrain((float)x, (float)z + 1);

		glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
		normal = glm::normalize(normal);
		return normal;
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}

// -------------------
// Author: Rony Hanna
// Description: Function that gets the heights of the terrain grids
// -------------------
float Terrain::GetHeightOfTerrain(float _X, float _Z)
{
	float result = 0.0f;

	float terrainX = _X - m_terrainXPos;
	float terrainZ = _Z - m_terrainZPos;

	// calculate length of grid square
	float gridSquareLength = m_terrainLength * m_cellSpacing / ((float)m_terrainWidth - 1);

	// Check which grid square the player is in
	int gridX = (int)std::floor(terrainX / gridSquareLength);
	int gridZ = (int)std::floor(terrainZ / gridSquareLength);

	// Check if position is on the terrain
	if (gridX >= m_terrainWidth - 1 || gridZ >= m_terrainLength - 1 || gridX < 0 || gridZ < 0)
	{
		return 0.0f;
	}

	// Find out where the player's position is on the terrain
	float xCoord = std::fmod(terrainX, gridSquareLength) / gridSquareLength;
	float zCoord = std::fmod(terrainZ, gridSquareLength) / gridSquareLength;

	// Top triangle of the quad, else the bottom triangle of the quad
	if (xCoord <= (1 - zCoord))
	{
		result = BarryCentric(glm::vec3(0, m_vHeights[gridX][gridZ] * m_fTerrainHeight, 0),
			glm::vec3(1, m_vHeights[gridX + 1][gridZ] * m_fTerrainHeight, 0),
			glm::vec3(0, m_vHeights[gridX][gridZ + 1] * m_fTerrainHeight, 1),
			glm::vec2(xCoord, zCoord));
	}
	else
	{
		result = BarryCentric(glm::vec3(1, m_vHeights[gridX + 1][gridZ] * m_fTerrainHeight, 0),
			glm::vec3(1, m_vHeights[gridX + 1][gridZ + 1] * m_fTerrainHeight, 1),
			glm::vec3(0, m_vHeights[gridX][gridZ + 1] * m_fTerrainHeight, 1),
			glm::vec2(xCoord, zCoord));
	}

	return result;
}

// -------------------
// Author: Rony Hanna
// Description: Helper function used to find the height of a triangle the player is currently on
// -------------------
float Terrain::BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

// -------------------
// Author: Rony Hanna
// Description: Function that binds the terrain vertex array object (VAO) and draws its vertex data
// -------------------
void Terrain::Draw(Camera& _cam, DirectionalLight* directionLight, PointLight* lamp, SpotLight* spotlight)
{
	m_terrainShader.ActivateProgram();

	// Texture properties
	m_terrainShader.SetInt("meshTexture", 0);
	m_terrainShader.SetInt("rTexture", 1);
	m_terrainShader.SetInt("gTexture", 2);
	m_terrainShader.SetInt("bTexture", 3);
	m_terrainShader.SetInt("blendMap", 4);
	m_terrainShader.SetInt("grassNormalMap", 5);

	// Activate all the textures
	for (unsigned int i = 0; i < 6; ++i)
		m_terrainTexture.ActivateTextures(i);

	// MVP transformation matrix
	m_terrainShader.SetMat4("model", m_model);
	m_terrainShader.SetMat4("view", _cam.GetViewMatrix());
	m_terrainShader.SetMat4("projection", _cam.GetProjectionMatrix());
	m_terrainShader.SetVec3("viewPos", _cam.GetCameraPos());

	// ----------------------
	// Lighting properties 
	// ----------------------

	// a. Directional Light
	m_terrainShader.SetVec3("dirLight.direction", directionLight->GetDirection());
	m_terrainShader.SetVec3("dirLight.ambient", directionLight->GetAmbient());
	m_terrainShader.SetVec3("dirLight.diffuse", directionLight->GetDiffuse());
	m_terrainShader.SetVec3("dirLight.specular", directionLight->GetSpecular());
	m_terrainShader.SetVec3("dirLight.lightColour", directionLight->GetColour());

	// b. Some light bulbs
	m_terrainShader.SetVec3("pointLight.position", lamp->GetPos());
	m_terrainShader.SetVec3("pointLight.ambient", lamp->GetAmbient());
	m_terrainShader.SetVec3("pointLight.diffuse", lamp->GetDiffuse());
	m_terrainShader.SetVec3("pointLight.specular", lamp->GetSpecular());
	m_terrainShader.SetVec3("pointLight.lightColour", lamp->GetColour());
	m_terrainShader.SetFloat("pointLight.constant", lamp->GetConstant());
	m_terrainShader.SetFloat("pointLight.linear", lamp->GetLinear());
	m_terrainShader.SetFloat("pointLight.quadratic", lamp->GetQuadratic());

	// c. Spotlight
	m_terrainShader.SetVec3("spotlight.position", spotlight->GetPosition());
	m_terrainShader.SetVec3("spotlight.direction", spotlight->GetDirection());
	m_terrainShader.SetVec3("spotlight.diffuse", spotlight->GetDiffuse());
	m_terrainShader.SetVec3("spotlight.specular", spotlight->GetSpecular());
	m_terrainShader.SetFloat("spotlight.constant", spotlight->GetConstant());
	m_terrainShader.SetFloat("spotlight.linear", spotlight->GetLinear());
	m_terrainShader.SetFloat("spotlight.quadratic", spotlight->GetQuadratic());
	m_terrainShader.SetFloat("spotlight.cutOff", glm::cos(glm::radians(spotlight->GetCutOff())));
	m_terrainShader.SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(spotlight->GetOuterCutOff())));

	// Fog effect
	if (m_fog)
		m_terrainShader.SetBool("fogActive", true);
	else
		m_terrainShader.SetBool("fogActive", false);

	// Draw the terrain
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[ELEMENT_BUFFER]);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_terrainShader.DeactivateProgram();
}