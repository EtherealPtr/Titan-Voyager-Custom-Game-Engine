#include "Mesh.h"
#include "Player.h"

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures, bool instancing)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	CreateMesh(instancing);
}

void Mesh::Draw(Camera& camera, Shader shaderProgram, bool instancing, glm::vec3& pos, glm::vec3& rot, float amountOfRotation, glm::vec3& scale, bool bDrawRelativeToCamera, bool bUseSpotlight)
{
	shaderProgram.ActivateProgram();

	glm::mat4 model(1.0f);
	glm::mat4 translation = glm::translate(pos);
	glm::mat4 rotation = glm::rotate(amountOfRotation, rot);
	glm::mat4 scaleMat = glm::scale(scale);

	if (bDrawRelativeToCamera)
	{
		glm::mat4 invViewMat = glm::inverse(camera.GetViewMatrix());
		model = invViewMat * translation * rotation * scaleMat;
	}
	else
	{
		model = translation * rotation * scaleMat;
	}

	shaderProgram.SetMat4("model", model);
	shaderProgram.SetVec3("lightPos", glm::vec3(camera.GetCameraPos().x, camera.GetCameraPos().y + 5.0f, camera.GetCameraPos().z));
	shaderProgram.SetVec3("viewPos", glm::vec3(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z));
	shaderProgram.SetBool("EnableSpotlight", bUseSpotlight);

	if (Player::GetInstance().GetSpotLight() != nullptr && bUseSpotlight)
	{
		shaderProgram.SetVec3("spotlight.position", Player::GetInstance().GetSpotLight()->GetPosition());
		shaderProgram.SetVec3("spotlight.direction", Player::GetInstance().GetSpotLight()->GetDirection());
		shaderProgram.SetVec3("spotlight.diffuse", Player::GetInstance().GetSpotLight()->GetDiffuse());
		shaderProgram.SetVec3("spotlight.specular", Player::GetInstance().GetSpotLight()->GetSpecular());
		shaderProgram.SetFloat("spotlight.constant", Player::GetInstance().GetSpotLight()->GetConstant());
		shaderProgram.SetFloat("spotlight.linear", Player::GetInstance().GetSpotLight()->GetLinear());
		shaderProgram.SetFloat("spotlight.quadratic", Player::GetInstance().GetSpotLight()->GetQuadratic());
		shaderProgram.SetFloat("spotlight.cutOff", glm::cos(glm::radians(Player::GetInstance().GetSpotLight()->GetCutOff())));
		shaderProgram.SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(Player::GetInstance().GetSpotLight()->GetOuterCutOff())));
	}

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
	
		std::string number;
		std::string name = m_textures[i].m_type;
	
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
	
		glm::mat4 projMat = camera.GetProjectionMatrix();
		glm::mat4 viewMat = camera.GetViewMatrix();

		shaderProgram.SetMat4("projection", projMat);
		shaderProgram.SetMat4("view", viewMat);
		glUniform1i(glGetUniformLocation(shaderProgram.GetShaderProgram(), (name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	
	glBindVertexArray(m_vao);

	if (instancing)
	{
		glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_totalObjectsIns);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);

	// Unbind textures 
	for (GLuint i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::CreateMesh(bool instancing)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
 
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, m_Normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, m_TexCoords));

	if (instancing)
	{
		m_totalObjectsIns = 20000;
		unsigned int amount = m_totalObjectsIns;
		m_modelMatricesIns = new glm::mat4[amount];
		float radius = 400.0f;
		float offset = 60.0f;

		for (unsigned int i = 0; i < amount; ++i)
		{
			glm::mat4 model(1.0f);
			float x = 0.0f, y = 0.0f, z = 0.0f;

			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 300.0f;
			x = sin(angle) * radius + displacement;

			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 40.0f;
			y = displacement * 3.7f;

			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 390.0f;
			z = cos(angle) * radius + displacement;

			model = glm::translate(model, glm::vec3(x, y, z));

			float scale = (rand() % 25) / 100.0f + 0.35f;
			model = glm::scale(model, glm::vec3(scale));

			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.5f, 0.7f, 0.9f));

			m_modelMatricesIns[i] = model;
		}

		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &m_modelMatricesIns[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < 1; ++i)
		{
			glBindVertexArray(m_vao);

			GLsizei mat4Size = sizeof(glm::mat4);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glEnableVertexAttribArray(6);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)0);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(sizeof(glm::vec4)));
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(2 * sizeof(glm::vec4)));
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	glBindVertexArray(0);
}