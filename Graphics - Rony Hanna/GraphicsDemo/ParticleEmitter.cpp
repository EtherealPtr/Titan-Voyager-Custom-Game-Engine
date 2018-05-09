#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() :
	m_numberOfParticles(500)
{}

ParticleEmitter::~ParticleEmitter()
{}

void ParticleEmitter::Init(char* vs, char* fs, int numOfParticles)
{
	m_texture.GenerateTexture("fire");
	m_shader.CreateProgram(vs, fs); 

	m_numberOfParticles = numOfParticles;

	for (unsigned int i = 0; i < m_numberOfParticles; ++i)
	{
		m_position.push_back(glm::vec3(0.0f));
		Particle p; 
		m_particles.push_back(p);
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(TOTAL_BUFFERS, m_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VERTEX_VB]);
	glBufferData(GL_ARRAY_BUFFER, m_position.size() * sizeof(glm::vec3), &m_position[0], GL_STATIC_DRAW); 

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleEmitter::Render(Camera& cam, float dt)
{
	int c = 0;
	for (std::vector<Particle>::iterator iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		iter->Update(dt);
		m_position[c] = iter->GetPos();
		c++;
	}

	m_shader.ActivateProgram();

	glm::mat4 model(1.0f);
	m_shader.SetMat4("view", cam.GetViewMatrix());
	m_shader.SetMat4("projection", cam.GetProjectionMatrix());
	m_shader.SetMat4("model", model);

	glBindVertexArray(m_vao);
	m_texture.ActivateTexture();
	glDrawArrays(GL_TRIANGLES, 0, m_numberOfParticles);
	glBindVertexArray(0);

	m_shader.DeactivateProgram();
}