#include "Shape.h"

class Triangle : public Shape
{
public:
	virtual void InitVertexData() override
	{
		m_triVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_triVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_triVertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	}

	virtual std::vector<Vertex> GetVertexData() override
	{
		return m_triVertices;
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return m_triVertices.size();
	}

	virtual unsigned int* GetIndexData() override
	{
		return m_triIndices;
	}

	virtual unsigned int GetIndexDataCount() override
	{
		return (sizeof(m_triIndices) / sizeof(m_triIndices[0]));
	}

private:
	std::vector<Vertex> m_triVertices;
	unsigned int m_triIndices[3] =
	{
		0, 1, 2
	};
};

class Quad : public Shape
{
public:
	virtual void InitVertexData() override
	{
		m_quadVertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_quadVertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_quadVertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_quadVertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	}

	virtual std::vector<Vertex> GetVertexData() override
	{
		return m_quadVertices;
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return m_quadVertices.size();
	}

	virtual unsigned int* GetIndexData() override
	{
		return m_quadIndices;
	}

	virtual unsigned int GetIndexDataCount() override
	{
		return (sizeof(m_quadIndices) / sizeof(m_quadIndices[0]));
	}

private:
	std::vector<Vertex> m_quadVertices;
	unsigned int m_quadIndices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};
};

class Cube : public Shape
{
	virtual void InitVertexData() override
	{
		// Cube is drawn in a counter-clockwise winding format to allow for face culling
		// Front face
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

		// Back face
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));

		// Right face
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		// Left face
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));

		// Top face
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

		// Bottom face
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		m_cubeVertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	}

	virtual std::vector<Vertex> GetVertexData() override
	{
		return m_cubeVertices;
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return m_cubeVertices.size();
	}

	virtual unsigned int* GetIndexData() override
	{
		return m_cubeIndices;
	}

	virtual unsigned int GetIndexDataCount() override
	{
		return (sizeof(m_cubeIndices) / sizeof(m_cubeIndices[0]));
	}

private:
	std::vector<Vertex> m_cubeVertices;
	unsigned int m_cubeIndices[36] =
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};
};

class Sphere : public Shape
{
	virtual void InitVertexData() override
	{
		float phyStep = m_PI / (float)m_stacks;
		float thetaStep = 2.0f * m_PI / (float)m_slices;

		for (int i = 0; i <= m_stacks; ++i) 
		{
			float phi = i * phyStep;

			for (int j = 0; j <= m_slices; ++j)
			{
				float theta = j * thetaStep;

				float x = cosf(theta) * sinf(phi);
				float y = cosf(phi);
				float z = sinf(theta) * sinf(phi);

				m_sphereVertices.push_back(Vertex(glm::vec3(x * m_radius, y * m_radius, z * m_radius), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(theta / (m_PI * 2.0f), phi / m_PI), glm::vec3(x, y, z)));
			}
		}

		for (int i = 0; i < m_slices * m_stacks + m_slices; ++i) 
		{
			m_sphereIndices.push_back(i);
			m_sphereIndices.push_back(i + m_slices + 1);
			m_sphereIndices.push_back(i + m_slices);

			m_sphereIndices.push_back(i + m_slices + 1);
			m_sphereIndices.push_back(i);
			m_sphereIndices.push_back(i + 1);
		}
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return m_sphereVertices.size();
	}

	virtual std::vector<Vertex> GetVertexData() override
	{
		return m_sphereVertices;
	}

	virtual unsigned int* GetIndexData() override
	{
		return &m_sphereIndices[0];
	}

	virtual unsigned int GetIndexDataCount() override
	{
		return m_sphereIndices.size();
	}

private:
	int m_stacks = 75;
	int m_slices = 75;
	int m_radius = 3;
	const float m_PI = 3.14159265359f;

	std::vector<Vertex> m_sphereVertices;
	std::vector<unsigned int> m_sphereIndices;
};

Shape* Shape::CreateShape(unsigned int shapeID)
{
	switch (shapeID)
	{
	case 0:
		return new Triangle;
		break;

	case 1:
		return new Quad;
		break;

	case 2:
		return new Cube;
		break;

	case 3:
		return new Sphere;
		break;

	case 4:
		//return new Circle;
		break;

	default:
		break;
	}

	return nullptr;
}