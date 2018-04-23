#include "Shape.h"

// -------------------
// Author: Rony Hanna
// Description: Helper function that calculates the tangent of a given vertex of a triangle  
// -------------------
std::vector<glm::vec3> CalculateVertexTangent(std::vector<glm::vec3> positions, std::vector<glm::vec2> textures)
{
	std::vector<glm::vec3> tangents;
	glm::vec3 tan1, tan2;

	// Triangle 1
	glm::vec3 edge1 = positions.at(1) - positions.at(0);
	glm::vec3 edge2 = positions.at(2) - positions.at(0);
	glm::vec2 deltaUV1 = textures.at(1) - textures.at(0);
	glm::vec2 deltaUV2 = textures.at(2) - textures.at(0);

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tan1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tan1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tan1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tan1 = glm::normalize(tan1);
	tangents.push_back(tan1);

	// Triangle 2
	edge1 = positions.at(2) - positions.at(0);
	edge2 = positions.at(3) - positions.at(0);
	deltaUV1 = textures.at(2) - textures.at(0);
	deltaUV2 = textures.at(3) - textures.at(0);

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tan2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tan2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tan2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tan2 = glm::normalize(tan2);
	tangents.push_back(tan2);

	return tangents;
}

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
		std::vector<glm::vec3> frontFacePos;
		frontFacePos.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		frontFacePos.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		frontFacePos.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		frontFacePos.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));

		std::vector<glm::vec2> textures;
		textures.push_back(glm::vec2(0.0f, 0.0f));
		textures.push_back(glm::vec2(1.0f, 0.0f));
		textures.push_back(glm::vec2(1.0f, 1.0f));
		textures.push_back(glm::vec2(0.0f, 1.0f));

		std::vector<glm::vec3> frontFaceTan = CalculateVertexTangent(frontFacePos, textures);

		m_cubeVertices.push_back(Vertex(frontFacePos.at(0), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(0), glm::vec3(0.0f, 0.0f, 1.0f), frontFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(frontFacePos.at(1), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(1), glm::vec3(0.0f, 0.0f, 1.0f), frontFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(frontFacePos.at(2), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(2), glm::vec3(0.0f, 0.0f, 1.0f), frontFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(frontFacePos.at(3), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(3), glm::vec3(0.0f, 0.0f, 1.0f), frontFaceTan.at(1)));

		// Calculate tangents for back face
		std::vector<glm::vec3> backFacePos;
		backFacePos.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
		backFacePos.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		backFacePos.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
		backFacePos.push_back(glm::vec3(0.5f, 0.5f, -0.5f));

		std::vector<glm::vec3> backFaceTan = CalculateVertexTangent(backFacePos, textures);

		// Back face
		m_cubeVertices.push_back(Vertex(backFacePos.at(0), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(0), glm::vec3(0.0f, 0.0f, -1.0f), backFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(backFacePos.at(1), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(1), glm::vec3(0.0f, 0.0f, -1.0f), backFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(backFacePos.at(2), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(2), glm::vec3(0.0f, 0.0f, -1.0f), backFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(backFacePos.at(3), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(3), glm::vec3(0.0f, 0.0f, -1.0f), backFaceTan.at(1)));

		// Calculate tangents for right face
		std::vector<glm::vec3> rightFacePos;
		rightFacePos.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		rightFacePos.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
		rightFacePos.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
		rightFacePos.push_back(glm::vec3(0.5f, 0.5f, 0.5f));

		std::vector<glm::vec3> rightFaceTan = CalculateVertexTangent(rightFacePos, textures);

		// Right face
		m_cubeVertices.push_back(Vertex(rightFacePos.at(0), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(0), glm::vec3(1.0f, 0.0f, 0.0f), rightFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(rightFacePos.at(1), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(1), glm::vec3(1.0f, 0.0f, 0.0f), rightFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(rightFacePos.at(2), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(2), glm::vec3(1.0f, 0.0f, 0.0f), rightFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(rightFacePos.at(3), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(3), glm::vec3(1.0f, 0.0f, 0.0f), rightFaceTan.at(1)));

		// Calculate tangents for left face
		std::vector<glm::vec3> leftFacePos;
		leftFacePos.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		leftFacePos.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		leftFacePos.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
		leftFacePos.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));

		std::vector<glm::vec3> leftFaceTan = CalculateVertexTangent(leftFacePos, textures);

		// Left face
		m_cubeVertices.push_back(Vertex(leftFacePos.at(0), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(0), glm::vec3(-1.0f, 0.0f, 0.0f), leftFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(leftFacePos.at(1), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(1), glm::vec3(-1.0f, 0.0f, 0.0f), leftFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(leftFacePos.at(2), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(2), glm::vec3(-1.0f, 0.0f, 0.0f), leftFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(leftFacePos.at(3), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(3), glm::vec3(-1.0f, 0.0f, 0.0f), leftFaceTan.at(1)));

		// Calculate tangents for top face
		std::vector<glm::vec3> topFacePos;
		topFacePos.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
		topFacePos.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		topFacePos.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
		topFacePos.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));

		std::vector<glm::vec3> topFaceTan = CalculateVertexTangent(topFacePos, textures);

		// Top face
		m_cubeVertices.push_back(Vertex(topFacePos.at(0), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(0), glm::vec3(0.0f, 1.0f, 0.0f), topFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(topFacePos.at(1), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(1), glm::vec3(0.0f, 1.0f, 0.0f), topFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(topFacePos.at(2), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(2), glm::vec3(0.0f, 1.0f, 0.0f), topFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(topFacePos.at(3), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(3), glm::vec3(0.0f, 1.0f, 0.0f), topFaceTan.at(1)));

		// Calculate tangents for bottom face
		std::vector<glm::vec3> bottomFacePos;
		bottomFacePos.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		bottomFacePos.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		bottomFacePos.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		bottomFacePos.push_back(glm::vec3(0.5f, -0.5f, -0.5f));

		std::vector<glm::vec3> bottomFaceTan = CalculateVertexTangent(bottomFacePos, textures);

		// Bottom face
		m_cubeVertices.push_back(Vertex(bottomFacePos.at(0), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(0), glm::vec3(0.0f, -1.0f, 0.0f), bottomFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(bottomFacePos.at(1), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(1), glm::vec3(0.0f, -1.0f, 0.0f), bottomFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(bottomFacePos.at(2), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(2), glm::vec3(0.0f, -1.0f, 0.0f), bottomFaceTan.at(0)));
		m_cubeVertices.push_back(Vertex(bottomFacePos.at(3), glm::vec3(1.0f, 0.0f, 0.0f), textures.at(3), glm::vec3(0.0f, -1.0f, 0.0f), bottomFaceTan.at(1)));
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

	default:
		break;
	}

	return nullptr;
}