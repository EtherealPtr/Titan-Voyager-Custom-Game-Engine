#include "Cloth.h"
#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/glm-0.9.9-a2/glm/gtc/type_ptr.hpp"

Cloth::Cloth() :
	m_position(1.0f)
{}

Cloth::~Cloth()
{}

void Cloth::Configure(float w, float h, int totalParticlesW, int totalParticlesH)
{
	// Create shader program
	m_shader.CreateProgram("res/Shaders/Cloth Shaders/VertexShader.vs", "res/Shaders/Cloth Shaders/FragmentShader.fs");

	// Create texture
	m_textureComponent.GenerateTexture("clothTex");

	// Allocate enough room for particles of cloth in the vector
	m_particles.resize(totalParticlesW * totalParticlesH);
	m_numParticlesWidth = totalParticlesW;
	m_numParticlesHeight = totalParticlesH;

	for (unsigned int i = 0; i < totalParticlesW; ++i)
	{
		for (unsigned int j = 0; j < totalParticlesH; ++j)
		{
			glm::vec3 pos = glm::vec3(w * (i / (float)m_numParticlesWidth), -h * (j / (float)totalParticlesH), 0);

			// Add particle at element (i, j)
			m_particles[j * totalParticlesW + i] = ClothParticle(pos); 
		}
	}
	
	// Connect close neighbours with constraints
	for (unsigned int i = 0; i < totalParticlesW; ++i)
	{
		for (unsigned int j = 0; j < totalParticlesH; ++j)
		{
			if (i < totalParticlesW - 1)
				CreateConstraint(GetParticle(i, j), GetParticle(i + 1, j));

			if (j < totalParticlesH - 1)
				CreateConstraint(GetParticle(i, j), GetParticle(i, j + 1));

			if (i < totalParticlesW - 1 && j < totalParticlesH - 1)
				CreateConstraint(GetParticle(i, j), GetParticle(i + 1, j + 1));

			if (i < totalParticlesW - 1 && j < totalParticlesH - 1)
				CreateConstraint(GetParticle(i + 1, j), GetParticle(i, j + 1));
		}
	}

	// Connect secondary neighbours with constraints
	for (unsigned int i = 0; i < totalParticlesW; ++i)
	{
		for (unsigned int j = 0; j < totalParticlesH; ++j)
		{
			if (i < totalParticlesW - 2)
				CreateConstraint(GetParticle(i, j), GetParticle(i + 2, j));
			
			if (j < totalParticlesH - 2)
				CreateConstraint(GetParticle(i, j), GetParticle(i, j + 2));

			if (i < totalParticlesW - 2 && j < totalParticlesH - 2)
				CreateConstraint(GetParticle(i, j), GetParticle(i + 2, j + 2));

			if (i < totalParticlesW - 2 && j < totalParticlesH - 2)
				CreateConstraint(GetParticle(i + 2, j), GetParticle(i, j + 2));
		}
	}

	// Pin particles
	for (unsigned int i = 0; i < 3; ++i)
	{
		// Top left particles
		GetParticle(i, 0)->Pin();
		
		for (unsigned int j = 0; j < totalParticlesH; ++j)
		{
			if (j >= totalParticlesH - 3)
			{
				// Bottom left particles
				GetParticle(i, j)->Pin();
			}
		}
	}
}

// -------------------
// Author: Rony Hanna
// Description: Function that renders the cloth (sets vertex buffers and sends it to GPU for rendering)
// -------------------
void Cloth::Draw(Camera& cam)
{
	m_shader.ActivateProgram();
	m_textureComponent.ActivateTexture();

	// Reset normals
	for (auto p = m_particles.begin(); p != m_particles.end(); ++p)
		(*p).ZeroNormal();

	for (unsigned int i = 0; i < m_numParticlesWidth - 1; ++i)
	{
		for (unsigned int j = 0; j < m_numParticlesHeight - 1; ++j)
		{
			glm::vec3 normal = CalculateTriNormal(GetParticle(i + 1, j), GetParticle(i, j), GetParticle(i, j + 1));
			GetParticle(i+ 1, j)->AddToNormal(normal);
			GetParticle(i, j)->AddToNormal(normal);
			GetParticle(i, j + 1)->AddToNormal(normal);

			normal = CalculateTriNormal(GetParticle(i + 1, j + 1), GetParticle(i + 1, j), GetParticle(i, j + 1));
			GetParticle(i + 1, j + 1)->AddToNormal(normal);
			GetParticle(i + 1, j)->AddToNormal(normal);
			GetParticle(i, j + 1)->AddToNormal(normal);
		}
	}

	static GLuint vertexArrayObject = 0;
	static GLuint vertexBuffer = 0;
	static GLuint texture;
	static int elementSize;

	if (vertexArrayObject == 0)
	{
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		GLuint positionAttributeLocation = glGetAttribLocation(m_shader.GetShaderProgram(), "position");
		GLuint uvAttributeLocation = glGetAttribLocation(m_shader.GetShaderProgram(), "uv");
		GLuint normalAttributeLocation = glGetAttribLocation(m_shader.GetShaderProgram(), "normal");
		glEnableVertexAttribArray(positionAttributeLocation);
		glEnableVertexAttribArray(uvAttributeLocation);
		glEnableVertexAttribArray(normalAttributeLocation);
		glVertexAttribPointer(positionAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (const GLvoid *)0);
		glVertexAttribPointer(uvAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (const GLvoid *)sizeof(glm::vec3));
		glVertexAttribPointer(normalAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (const GLvoid *)(sizeof(glm::vec3) + sizeof(glm::vec2)));

		std::vector<int> indices;

		for (int j = 0; j < m_numParticlesHeight - 1; ++j) 
		{
			int index;

			if (j > 0) 
				indices.push_back(j * m_numParticlesWidth);  

			for (int i = 0; i <= m_numParticlesWidth - 1; ++i) 
			{
				index = j * m_numParticlesWidth + i;
				indices.push_back(index);
				indices.push_back(index + m_numParticlesWidth);
			}

			if (j + 1 < m_numParticlesHeight - 1) 
				indices.push_back(index + m_numParticlesWidth);  
		}

		elementSize = indices.size();

		GLuint elementArrayBuffer;
		glGenBuffers(1, &elementArrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * sizeof(int), &(indices[0]), GL_STATIC_DRAW);
	}

	std::vector<Vert> vertexData;

	for (int j = 0; j < m_numParticlesHeight; ++j)
	{
		for (int i = 0; i < m_numParticlesWidth; ++i)
		{
			glm::vec2 uv(i / (m_numParticlesWidth - 1.0f), j / (m_numParticlesHeight - 1.0f));
			AddTriangle(GetParticle(i, j), uv, vertexData);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vert), glm::value_ptr(vertexData[0].m_pos), GL_STREAM_DRAW);

	glm::mat4 view = cam.GetViewMatrix();
	glm::mat4 model = glm::translate(m_position);
	glm::mat4 mvp = cam.GetProjectionMatrix() * view * model;
	glUniformMatrix4fv(glGetUniformLocation(m_shader.GetShaderProgram(), "mvp"), 1, false, glm::value_ptr(mvp));
	glUniformMatrix4fv(glGetUniformLocation(m_shader.GetShaderProgram(), "view"), 1, false, glm::value_ptr(view));

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLE_STRIP, elementSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_shader.DeactivateProgram();
}

// -------------------
// Author: Rony Hanna
// Description: Function that updates the particles of a cloth each frame
// -------------------
void Cloth::Update()
{
	for (unsigned int i = 0; i < 3; ++i) 
	{
		for (auto iter = m_constraints.begin(); iter != m_constraints.end(); ++iter)
			(*iter).SatisfyConstraint();
	}

	// Calculate the position of each particle  
	for (auto p = m_particles.begin(); p != m_particles.end(); ++p)
		(*p).VerletIntegration(); 
}

// -------------------
// Author: Rony Hanna
// Description: Function that adds a directional force to all particles
// -------------------
void Cloth::AddForce(glm::vec3 dir)
{
	for (auto p = m_particles.begin(); p != m_particles.end(); ++p)
		(*p).AddForce(dir);
}

// -------------------
// Author: Rony Hanna
// Description: Function that adds a wind force to all particles
// -------------------
void Cloth::WindForce(glm::vec3 dir)
{
	for (unsigned int i = 0; i < m_numParticlesWidth - 1; ++i)
	{
		for (unsigned int j = 0; j < m_numParticlesHeight - 1; ++j)
		{
			AddWindForce(GetParticle(i + 1, j), GetParticle(i, j), GetParticle(i, j + 1), dir);
			AddWindForce(GetParticle(i + 1, j + 1), GetParticle(i + 1, j), GetParticle(i, j + 1), dir);
		}
	}
}

// -------------------
// Author: Rony Hanna
// Description: Function that calculates the normal vector of triangle where the normal vector is equal to the area of the parallelogram defined by the particles
// -------------------
glm::vec3 Cloth::CalculateTriNormal(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3)
{
	glm::vec3 pos1 = p1->GetPos();
	glm::vec3 pos2 = p2->GetPos();
	glm::vec3 pos3 = p3->GetPos();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3 v2 = pos3 - pos1;

	return glm::cross(v1, v2);
}

// -------------------
// Author: Rony Hanna
// Description: Function that calculates the wind force for a triangle
// -------------------
void Cloth::AddWindForce(ClothParticle* p1, ClothParticle* p2, ClothParticle* p3, glm::vec3 windDir)
{
	glm::vec3 normal = CalculateTriNormal(p1, p2, p3);
	glm::vec3 normalFinal = glm::normalize(normal); 
	glm::vec3 force = normal * glm::dot(normalFinal, windDir);

	p1->AddForce(force); 
	p2->AddForce(force);
	p3->AddForce(force);
}

// -------------------
// Author: Rony Hanna
// Description: Function that adds a vertex into a vector of vertex data in order to form and draw a triangle
// -------------------
void Cloth::AddTriangle(ClothParticle* p1, glm::vec2 uv, std::vector<Vert>& vertexData)
{
	Vert v1 = { p1->GetPos(), uv, p1->GetNormal() };
	vertexData.push_back(v1);
}