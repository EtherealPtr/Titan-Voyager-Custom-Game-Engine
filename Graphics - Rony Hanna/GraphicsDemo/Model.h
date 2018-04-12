#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "Mesh.h"
#include <string>

class Model
{
public:
	Model() {}

	GLint TextureFromFile(const char* path, std::string directory);
	void Init(GLchar* path, Camera camera, char* vs, char* fs, bool instancing);
	void Draw(Camera cam);
	void DrawInstanced(Camera cam);

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
};

#endif // !__MODEL_H__