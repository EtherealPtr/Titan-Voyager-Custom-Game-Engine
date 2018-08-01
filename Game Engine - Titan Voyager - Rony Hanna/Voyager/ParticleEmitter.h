#pragma once
#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

#include "Texture.h"
#include "Particle.h"
#include "Camera.h"
#include "Shader.h"
#include "Transformation.h"
#include <vector>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void Init(char* vs, char* gs, char* fs, int numberOfParticles, char* textureId);
	void Render(Camera& camera, float dt, glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f));

	std::vector<Particle>& GetParticle() { return m_particles; }
	std::vector<glm::vec3>& GetPos() { return m_position; }

private:
	enum { VERTEX_VB, COLOR_VB, TOTAL_BUFFERS };

	Transform m_transform;
	GLuint m_vao, m_vbo[TOTAL_BUFFERS], m_textureID;
	Texture m_texture;
	Shader m_shader;
	float m_numberOfParticles;

	std::vector<Particle> m_particles;
	std::vector<glm::vec3> m_position;
};

#endif // !__PARTICLEEMITTER_H__