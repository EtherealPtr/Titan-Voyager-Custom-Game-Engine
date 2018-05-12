#include "ParticleEmitter.h"
 
ParticleEmitter::ParticleEmitter() :
	m_numberOfParticles(500)
{}

ParticleEmitter::~ParticleEmitter()
{}

void ParticleEmitter::Init(char* vs, char* gs, char* fs, int numOfParticles)
{
	m_texture.GenerateTexture("fire");
	m_shader.CreateProgram(vs, gs, fs); 

	m_numberOfParticles = numOfParticles;
 
	for (unsigned int i = 0; i < m_numberOfParticles; ++i)
	{
		m_position.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
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
	m_shader.ActivateProgram();
	std::vector<Particle>::iterator iter = m_particles.begin();

	while (iter != m_particles.end())
	{
		// Check if the particle's dead, if so delete it, otherwise update its position and send its updated state to the GPU
		if (!iter->Update(dt))
		{
			iter = m_particles.erase(iter);
		}
		else
		{
			m_transform.GetPos() = iter->GetPos();

			glm::mat4 model(1.0f);
			glm::mat4 translation = glm::translate(glm::vec3(m_transform.GetPos().x, m_transform.GetPos().y, 0.0f));
			glm::mat4 scaleMat = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
			model = scaleMat * translation;

			m_shader.SetMat4("view", cam.GetViewMatrix());
			m_shader.SetMat4("projection", cam.GetProjectionMatrix());
			m_shader.SetMat4("model", model);
			m_shader.SetVec4("vertexColor", iter->GetCol());

			glBindVertexArray(m_vao);
			m_texture.ActivateTexture();
			glDrawArrays(GL_POINTS, 0, m_numberOfParticles);
			glBindVertexArray(0);

			++iter;
		}
	}

	m_shader.DeactivateProgram();
}