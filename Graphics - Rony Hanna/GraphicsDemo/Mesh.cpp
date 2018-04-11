//#include "Mesh.h"
//
//Mesh::Mesh()
//{
//}
//
//Mesh::~Mesh()
//{
//}
//
//Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures)
//{
//	this->m_vertices = vertices;
//	this->m_indices = indices;
//	this->m_textures = textures;
//
//	InitMesh();
//}
//
//void Mesh::InitMesh()
//{
//	glGenVertexArrays(1, &m_VAO);
//	glGenBuffers(1, &m_VBO);
//	glGenBuffers(1, &m_EBO);
//
//	glBindVertexArray(m_VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glEnableVertexAttribArray(2);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, m_normal));
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, m_texCoord));
//	
//	glBindVertexArray(0);
//}
//
//void Mesh::Draw(Shader shader)
//{
//	unsigned int diffuseNr = 1;
//	unsigned int specularNr = 1;
//
//	for (unsigned int i = 0; i < m_textures.size(); ++i)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//
//		std::string number;
//		std::string name = m_textures[i].type;
//
//		if (name == "texture_diffuse")
//			number = std::to_string(diffuseNr++);
//		else if (name == "texture_specular")
//			number = std::to_string(specularNr++);
//
//		this->m_shader.SetFloat(("material." + name + number).c_str(), i);
//		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
//	}
//
//	glBindVertexArray(m_VAO);
//	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	glActiveTexture(GL_TEXTURE0);
//}