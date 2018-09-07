#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Vertices.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"
#include "Camera.h"
#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"
#include <vector>
#include "Transformation.h"
#include "SpotLight.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void CreateGameObj(std::vector<Vertex>, GLuint numOfVertices, unsigned int* indices, GLuint numOfIndices, bool bInstancing = false);
	void Draw(Camera& cam, glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f), bool bNormalMapping = false, SpotLight* spotlight = nullptr, bool postProcessing = false, GLuint postProcessingTexId = 0);
	void Draw(glm::mat4 model, Camera& cam, glm::vec3 lightPos);
	void RawDraw();
	void DrawInstanced(glm::mat4 proj, glm::mat4 view);
	void SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	Shader& GetShaderComponent()			{ return shaderComponent; }
	Texture& GetTextureComponent()			{ return m_textureComponent; }
	Animation& GetAnimationComponent()		{ return m_animationComponent; }
	Transform& GetTransformComponent()		{ return m_transform; }
	
	void SetTextureId(char* texId);
	void SetObjectId(int id);
	char* GetTextureId() const;
	int GetObjectId() const; 

private:
	enum { POSITION_VB, COLOR_VB, UV_VB, NORMALS_VB, TANGENT_VB, ELEMENT_VB, TOTAL_BUFFERS };

	char* m_texId;
	int m_objId;
	GLuint m_numOfVertices, m_numOfIndices;
	GLuint m_vao;
	GLuint m_vbo[TOTAL_BUFFERS];

private:
	glm::mat4* m_modelMatricesIns;
	Transform m_transform;
	Shader shaderComponent;
	Texture m_textureComponent;
	Animation m_animationComponent;
};

#endif // !__GAMEOBJECT_H__