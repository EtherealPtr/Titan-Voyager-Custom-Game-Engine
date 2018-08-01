#include "Shader.h"
#include <fstream>
#include <iostream>

Shader::Shader()
{}

Shader::~Shader()
{}

void Shader::SetBool(GLchar* name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_program, name), value);
}

void Shader::SetInt(GLchar* name, int value) const
{
	glUniform1i(glGetUniformLocation(m_program, name), value);
}

void Shader::SetFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(m_program, name), value);
}

void Shader::SetVec2(const char* name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(m_program, name), value.x, value.y);
}

void Shader::SetVec3(const char* name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(m_program, name), value.x, value.y, value.z);
}

void Shader::SetVec4(GLchar* name, glm::vec4 value) const
{
	glUniform4f(glGetUniformLocation(m_program, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(GLchar* name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, glm::value_ptr(value));
}

// -------------------
// Author: Rony Hanna
// Description: Function that creates a program with vertex and fragment shaders
// -------------------
GLuint Shader::CreateProgram(char* vertexShaderFile, char* fragmentShaderFile)
{
	std::string vertexShaderID = LoadShaderFromFile(vertexShaderFile);
	std::string fragmentShaderID = LoadShaderFromFile(fragmentShaderFile);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderID);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderID);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);

	glLinkProgram(m_program);

#pragma region ERROR_HANDLING
	{
		int programStatus = 0;
		char infoLogBuffer[512];
		glGetProgramiv(m_program, GL_LINK_STATUS, &programStatus);

		if (!programStatus)
		{
			glGetProgramInfoLog(m_program, 512, NULL, infoLogBuffer);
			std::cerr << "SHADER ERROR: Unable to link shader program.\n" << infoLogBuffer << "\n";
		}
	}
#pragma endregion // ERROR_HANDLING

	// Delete shaders once linked to the program object 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return m_program;
}

GLuint Shader::CreateProgram(char* vertexShader, char* geomShader, char* fragmentShader)
{
	int LinkingResult = 0;

	// Read shader files and save their IDs
	std::string VertexShaderID = LoadShaderFromFile(vertexShader);
	std::string GeomShaderID = LoadShaderFromFile(geomShader);
	std::string FragmentShaderID = LoadShaderFromFile(fragmentShader);

	GLuint VertexShader = CreateShader(GL_VERTEX_SHADER, VertexShaderID);
	GLuint GeometryShader = CreateShader(GL_GEOMETRY_SHADER, GeomShaderID);
	GLuint FragmentShader = CreateShader(GL_FRAGMENT_SHADER, FragmentShaderID);

	// Creates program, attach shaders, and link shaders
	m_program = glCreateProgram();
	glAttachShader(m_program, VertexShader);
	glAttachShader(m_program, GeometryShader);
	glAttachShader(m_program, FragmentShader);

	glLinkProgram(m_program);

#pragma region ERROR_HANDLING
	{
		int programStatus = 0;
		char infoLogBuffer[512];
		glGetProgramiv(m_program, GL_LINK_STATUS, &programStatus);

		if (!programStatus)
		{
			glGetProgramInfoLog(m_program, 512, NULL, infoLogBuffer);
			std::cerr << "SHADER ERROR: Unable to link shader program.\n" << infoLogBuffer << "\n";
		}
	}
#pragma endregion // ERROR_HANDLING

	return m_program;
}

// -------------------
// Author: Rony Hanna
// Description: Function that reads an external shader file
// -------------------
std::string Shader::LoadShaderFromFile(char* shaderFile)
{
	std::string ShaderCode;
	std::ifstream File(shaderFile, std::ios::in);

	if (!File.good())
	{
		std::cout << "SHADER ERROR: Unable to load shader: " << shaderFile << "\n";
		std::terminate();
	}

	File.seekg(0, std::ios::end);
	ShaderCode.resize((unsigned int)File.tellg());
	File.seekg(0, std::ios::beg);
	File.read(&ShaderCode[0], ShaderCode.size());

	File.close();
	return ShaderCode;
}

// -------------------
// Author: Rony Hanna
// Description: Function that creates and compiles a shader
// -------------------
GLuint Shader::CreateShader(GLenum shaderType, std::string source)
{
	m_shaderID = glCreateShader(shaderType);

	const char* ShaderCodePtr = source.c_str();
	const int ShaderCodeSize = source.size();

	glShaderSource(m_shaderID, 1, &ShaderCodePtr, &ShaderCodeSize);
	glCompileShader(m_shaderID);

#pragma region ERROR_HANDLING
	{
		int compileStatus = 0;
		char infoLogBuffer[512];
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compileStatus);

		if (!compileStatus)
		{
			glGetShaderInfoLog(m_shaderID, 512, NULL, infoLogBuffer);
			std::cerr << "SHADER ERROR: Unable to compile shader.\n" << infoLogBuffer << "\n";
		}
	}
#pragma endregion // ERROR_HANDLING

	return m_shaderID;
}