#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include "Dependencies\assimp\include\assimp\types.h"
#include "Dependencies\assimp\include\assimp\Importer.hpp"
#include "Dependencies\assimp\include\assimp\scene.h"
#include "Dependencies\assimp\include\assimp\postprocess.h"
#include "Dependencies\glm-0.9.9-a2\glm\glm.hpp"
#include "Dependencies\glm-0.9.9-a2\glm\gtx\transform.hpp"
#include <string>
#include <vector>
#include "Shader.h"
#include <sstream>
#include "Camera.h"
#include <cstdlib>
#include <ctime>
#include "Transformation.h"
#include <memory>

struct MeshVertex 
{
public:
	MeshVertex() {};
	~MeshVertex() {};

	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_TexCoords;
};

struct MeshTexture 
{
public:
	MeshTexture() {};
	~MeshTexture() {};

	GLuint m_id;
	std::string m_type;
	aiString m_path;
};

class Mesh 
{
public:
	Mesh(std::vector<MeshVertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures, bool instancing);

	glm::mat4* GetModelMatIns() { return m_modelMatricesIns; }

	void SetTransform(Transform& transform) { m_transform = transform; }
	void Draw(Camera& camera, Shader program, bool instancing, glm::vec3& pos = glm::vec3(1.0f), glm::vec3& rot = glm::vec3(1.0f), float amountOfRotation = 1.0f, 
			  glm::vec3& scale = glm::vec3(1.0f), bool bDrawRelativeToCamera = false, bool bUseSpotlight = false);

private:
	GLuint m_vao, m_vbo, m_ebo;
	Transform m_transform;
	glm::mat4* m_modelMatricesIns;
	float m_totalObjectsIns;
	std::vector<MeshVertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<MeshTexture> m_textures;

	void CreateMesh(bool instancing);
};

#endif // !__MESH_H__