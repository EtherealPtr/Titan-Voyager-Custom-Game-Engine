#include "Model.h"
#include "Dependencies\soil\include\SOIL.h"

GLint Model::TextureFromFile(const char* path, std::string directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}

void Model::Init(GLchar* path, Camera& camera, char* vs, char* fs, bool instancing)
{
	m_shader.CreateProgram(vs, fs);
	m_camera = camera;
	m_instancing = instancing;
	loadModel(path);
}

void Model::Draw(Camera& cam, bool bDrawRelativeToCamera)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(cam, m_shader, false, m_position, m_rotation, m_rotationAngle, m_scale, bDrawRelativeToCamera, m_useSpotlight);
	}
}

void Model::Draw(Camera& cam, glm::vec3& pos, glm::vec3& rot, float amountOfRotation, glm::vec3& scale, bool bDrawRelativeToCamera)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(cam, m_shader, false, pos, rot, amountOfRotation, scale, bDrawRelativeToCamera, m_useSpotlight);
	}
}

void Model::DrawInstanced(Camera& cam)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(cam, m_shader, true);
	}
}

void Model::SetTransform(glm::vec3 pos, glm::vec3 rot, float rotAmountInDegrees, glm::vec3 scale)
{
	m_position = pos;
	m_rotation = rot;
	m_rotationAngle = rotAmountInDegrees;
	m_scale = scale;
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR: Unable to load model\n");
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
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
		vertex.m_Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.m_Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_TexCoords = vec;
		}
		else
			vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);

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

		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures, m_instancing);
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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
			texture.m_id = TextureFromFile(str.C_Str(), directory);
			texture.m_type = typeName;
			texture.m_path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}