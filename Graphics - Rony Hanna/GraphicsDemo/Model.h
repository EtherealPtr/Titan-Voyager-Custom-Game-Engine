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
	void Init(GLchar* path, Camera* camera, GLuint program, bool instancing);
	void Draw();
	void DrawInstanced();

	std::vector<Mesh> meshes;
	Camera* camera;
	GLuint program;
	bool m_instancing = false;
	std::vector<MeshTexture> textures_loaded;	 

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	std::string directory;
};

#endif // !__MODEL_H__