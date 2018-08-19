#include "ParticleEmitter.h"
 
ParticleEmitter::ParticleEmitter() :
	m_numberOfParticles(500)
{}

ParticleEmitter::~ParticleEmitter()
{}

void ParticleEmitter::Init(char* vs, char* gs, char* fs, int numOfParticles, char* textureId)
{
	m_texture.GenerateTexture(textureId);
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

void ParticleEmitter::Render(Camera& cam, float dt, glm::vec3 origin)
{
	m_shader.ActivateProgram();
	glm::mat4 model = glm::mat4(1.0f);

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
			iter->SetOrigin(origin);
			m_transform.GetPos() = iter->GetPos();

			model = glm::translate(glm::vec3(m_transform.GetPos().x + 2.7f, m_transform.GetPos().y - 2.5f, m_transform.GetPos().z));

			m_shader.SetMat4("model", model);
			m_shader.SetMat4("view", cam.GetViewMatrix());
			m_shader.SetMat4("projection", cam.GetProjectionMatrix());

			glBindVertexArray(m_vao);
			m_texture.ActivateTexture();
			glDrawArrays(GL_POINTS, 0, m_numberOfParticles);
			glBindVertexArray(0);

			++iter;
		}
	}

	m_shader.DeactivateProgram();
}
