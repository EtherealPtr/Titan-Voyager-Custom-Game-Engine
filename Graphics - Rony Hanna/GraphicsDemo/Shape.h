#pragma once
#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "Vertices.h"
#include <vector>

class Shape
{
public:
	static Shape* CreateShape(unsigned int shapeID);

	virtual std::vector<Vertex> GetVertexData() = 0;
	virtual unsigned int GetVertexDataCount() = 0;
	virtual unsigned int* GetIndexData() = 0;
	virtual unsigned int GetIndexDataCount() = 0;
	virtual void InitVertexData() = 0;
};

#endif // !__SHAPE_H__