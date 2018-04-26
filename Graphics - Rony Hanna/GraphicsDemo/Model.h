#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "Mesh.h"
#include <string>
#include "Transformation.h"

class Model
{
public:
	Model() {}

	GLint TextureFromFile(const char* path, std::string directory);
	void Init(GLchar* path, Camera& camera, char* vs, char* fs, bool instancing);
	void Draw(Camera& cam, glm::vec3& pos = glm::vec3(1.0f), glm::vec3& rot = glm::vec3(1.0f), float amountOfRotation = 0.0f, glm::vec3& scale = glm::vec3(1.0f), bool bDrawRelativeToCamera = false);
	void DrawInstanced(Camera& cam);
	void SetTransform(Transform& transform) { m_transform = transform; }
	Shader& GetShaderProgram() { return m_shader; }

	std::vector<Mesh> meshes;
	GLuint program;
	bool m_instancing = false;
	std::vector<MeshTexture> textures_loaded;	 

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	std::string directory;
	Shader m_shader;
	Camera m_camera;
	Transform m_transform;
};

#endif // !__MODEL_H__