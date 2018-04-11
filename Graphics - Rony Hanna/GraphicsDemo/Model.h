#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "Mesh.h"
#include <string>

class Model
{
public:
	Model() {}

	Camera* camera;
	GLuint program;

	GLint TextureFromFile(const char* path, std::string directory);

	void Init(GLchar* path, Camera* camera, GLuint program)
	{
		this->program = program;
		this->camera = camera;
		this->loadModel(path);
	}

	void Draw()
	{
		for (GLuint i = 0; i < this->meshes.size(); i++) 
		{
			this->meshes[i].Draw(camera, program);
		}
	}

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<MeshTexture> textures_loaded;	 

	void loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  
			return;

		this->directory = path.substr(0, path.find_last_of('/'));
		this->processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}

		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<MeshVertex> vertices;
		std::vector<GLuint> indices;
		std::vector<MeshTexture> textures;

		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			MeshVertex vertex;
			glm::vec3 vector; 
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			if (mesh->mTextureCoords[0]) 
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<MeshTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<MeshTexture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<MeshTexture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			GLboolean skip = false;

			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].m_path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;  
					break;
				}
			}

			if (!skip)
			{  
				MeshTexture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.m_path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture); 
			}
		}
		return textures;
	}
};

#endif // !__MODEL_H__