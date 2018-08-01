#pragma once
#ifndef __CLOTH_H__
#define __CLOTH_H__

#include "ClothParticle.h"
#include <vector>
#include "Constraint.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

class Cloth
{
public:
	Cloth();
	~Cloth();

	void Configure(float w, float h, int totalParticlesW, int totalParticlesH);
	void Draw(Camera& cam);
	void Update();
	void AddForce(glm::vec3 dir);
	void WindForce(glm::vec3 dir);

	void SetPos(glm::vec3 pos) { m_position = pos; }
	Shader& GetShaderComponent() { return m_shader; }
	Texture& GetTextureComponent() { return m_textureComponent; }

private:
	struct Vert
	{
		glm::vec3 m_pos;
		glm::vec2 m_uv;
		glm::vec3 m_norm;
	};

	int m_numParticlesWidth, m_numParticlesHeight;
	std::vector<ClothParticle> m_particles;
	std::vector<Constraint> m_constraints;
	Shader m_shader;
	GLuint shaderId;
	Texture m_textureComponent;
	glm::vec3 m_position;

	// Private functions
	int GetParticleIndex(int x, int y) { return y * m_numParticlesWidth + x; }
	ClothParticle* GetParticle(int x, int y) { return &m_particles[GetParticleIndex(x, y)]; }
	void CreateConstraint(ClothParticle* p1, ClothParticle* p2) { m_constraints.push_back(Constraint(p1, p2)); }
	glm::vec3 CalculateTriNormal(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3);
	void AddWindForce(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3, glm::vec3 windDir);
	void AddTriangle(ClothParticle* p1, glm::vec2 uv, std::vector<Vert>& vertexData);
};

#endif // !__CLOTH_H__