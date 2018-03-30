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

	default:
		break;
	}

	return nullptr;
}
