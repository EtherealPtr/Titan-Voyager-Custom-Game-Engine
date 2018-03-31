#include "Shape.h"

class Triangle : public Shape
{
public:
	virtual void InitVertexData() override
	{
		m_triVertices[0] = { Vertex(glm::vec3(-0.5f, -0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_triVertices[1] = { Vertex(glm::vec3(0.5f, -0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.5f, 1.0f)) };
		m_triVertices[2] = { Vertex(glm::vec3(0.0f, 0.5f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
	}

	virtual Vertex* GetVertexData() override
	{
		return m_triVertices;
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return (sizeof(m_triVertices) / sizeof(m_triVertices[0]));
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
	Vertex m_triVertices[3];
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
		m_quadVertices[0] = { Vertex(glm::vec3(-0.5f, 0.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_quadVertices[1] = { Vertex(glm::vec3(0.5f, 0.0f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_quadVertices[2] = { Vertex(glm::vec3(0.5f, 0.5f, 0.0f),	    glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_quadVertices[3] = { Vertex(glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return (sizeof(m_quadVertices) / sizeof(m_quadVertices[0]));
	}

	virtual Vertex* GetVertexData() override
	{
		return m_quadVertices;
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
	Vertex m_quadVertices[4];
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
		m_cubeVertices[0] = { Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_cubeVertices[1] = { Vertex(glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_cubeVertices[2] = { Vertex(glm::vec3(0.5f, 0.5f, 0.5f),	    glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_cubeVertices[3] = { Vertex(glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
		
		// Back face
		m_cubeVertices[4] = { Vertex(glm::vec3(0.5f, -0.5f, -0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_cubeVertices[5] = { Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_cubeVertices[6] = { Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),	    glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_cubeVertices[7] = { Vertex(glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
		
		// Right face
		m_cubeVertices[8] = { Vertex(glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_cubeVertices[9] = { Vertex(glm::vec3(0.5f, -0.5f, -0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_cubeVertices[10] = { Vertex(glm::vec3(0.5f, 0.5f, -0.5f),	    glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_cubeVertices[11] = { Vertex(glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
		
		// Left face
		m_cubeVertices[12] = { Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_cubeVertices[13] = { Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_cubeVertices[14] = { Vertex(glm::vec3(-0.5f, 0.5f, 0.5f),     glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_cubeVertices[15] = { Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
	
		// Top face
		m_cubeVertices[16] = { Vertex(glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_cubeVertices[17] = { Vertex(glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_cubeVertices[18] = { Vertex(glm::vec3(0.5f, 0.5f, -0.5f),	    glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_cubeVertices[19] = { Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
		
		// Bottom face
		m_cubeVertices[20] = { Vertex(glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)) };
		m_cubeVertices[21] = { Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),    glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)) };
		m_cubeVertices[22] = { Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)) };
		m_cubeVertices[23] = { Vertex(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)) };
	}

	virtual unsigned int GetVertexDataCount() override
	{
		return (sizeof(m_cubeVertices) / sizeof(m_cubeVertices[0]));
	}

	virtual Vertex* GetVertexData() override
	{
		return m_cubeVertices;
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
	Vertex m_cubeVertices[24];
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

	default:
		break;
	}

	return nullptr;
}