#include "Renderer.h"
#include <iostream>

// -------------------
// Author: Rony Hanna
// Description: Destructore that free all allocated memory on the heap
// -------------------
Renderer::~Renderer()
{
	for (auto& mesh : m_meshes)
	{
		delete mesh;
		mesh = nullptr; 
	}

	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

// -------------------
// Author: Rony Hanna
// Description: Function that executes rendering components 
// -------------------
void Renderer::Run()
{
	window = nullptr;
	m_screenWidth = 800;
	m_screenHeight = 600;

	bool re = InitSDLAndOpenGL();

	if (!re)
	{
		std::cerr << "RENDERER ERROR: Unable to initialize SDL and OpenGL.\n";
		return;
	}
}

// -------------------
// Author: Rony Hanna
// Description: Function that set a number of OpenGL window attributes
// -------------------
void Renderer::SetWindowAttribs()
{
	int ret = 0;
	ret = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if (ret < 0)
		std::cerr << "SDL SetAttribute ERROR: SDL_GL_DOUBLEBUFFER\n";
	ret = SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	if (ret < 0)
		std::cerr << "SDL SetAttribute ERROR: SDL_GL_DEPTH_SIZE\n";
}

// -------------------
// Author: Rony Hanna
// Description: Function that initialize SDL, glew, and create an SDL window 
// -------------------
bool Renderer::InitSDLAndOpenGL()
{
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
	{
		std::cerr << "ERROR: SDL could not initialize, error: " << SDL_GetError() << "\n";
		return false;
	}

	SetWindowAttribs();

	window = SDL_CreateWindow("This is me MARIO!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cerr << "ERROR: SDL could not create window, error: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GLContext windowContext = SDL_GL_CreateContext(window);
	if (windowContext == nullptr)
	{
		std::cerr << "ERROR: SDL could not create GL context.\n";
		return false;
	}

	if (!(glewExperimental = true))
	{
		std::cerr << "Glew experimental error.\n";
		return false;
	}

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "ERROR: GLEW could not be initialized.\n";
		return false;
	}

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

// -------------------
// Author: Rony Hanna
// Description: Function that renders all the meshes in the vector
// -------------------
void Renderer::RenderMeshes()
{
	for (auto& mesh : m_meshes)
	{
		mesh->shaderComponent.ActivateProgram();
		mesh->Draw();
	}
}

// -------------------
// Author: Rony Hanna
// Description: Function that creates a mesh using a shape factory
// -------------------
void Renderer::InitMesh(GLuint meshType, char* textureId)
{
	m_shape = m_shape->CreateShape(meshType);
	m_shape->InitVertexData();

	Mesh* polygon = new Mesh();

	switch (meshType)
	{
	case TRIANGLE:
		polygon->CreateMesh(m_shape->GetVertexData(), m_shape->GetVertexDataCount(), m_shape->GetIndexData(), m_shape->GetIndexDataCount());
		polygon->shaderComponent.CreateProgram("Shaders/VertexShader.vs", "Shaders/FragmentShader.fs");
		polygon->m_textureComponent.GenerateTexture(textureId);
		polygon->SetTextureId(textureId);
		polygon->m_textureComponent.ActivateTexture();
		break;

	case QUAD:
		polygon->CreateMesh(m_shape->GetVertexData(), m_shape->GetVertexDataCount(), m_shape->GetIndexData(), m_shape->GetIndexDataCount());
		polygon->shaderComponent.CreateProgram("Shaders/VertexShader.vs", "Shaders/FragmentShader.fs");
		polygon->m_textureComponent.GenerateTexture(textureId);
		polygon->SetTextureId(textureId);
		polygon->m_textureComponent.ActivateTexture();
		break;
	}

	delete m_shape;
	m_shape = nullptr;

	m_meshes.push_back(polygon);
}

// -------------------
// Author: Rony Hanna
// Description: Function that returns a mesh component associated with a given texture id
// -------------------
Mesh& Renderer::GetComponent(char* textureId)
{
	Mesh* resultantMesh = nullptr;

	for (auto& mesh : m_meshes)
	{
		if (mesh->GetTextureId() == textureId)
		{
			resultantMesh = mesh;
			return *resultantMesh;
		}
	}

	std::cerr << "ERROR: Unable to find the mesh with texture id: " << textureId << ".\n";
	return *resultantMesh;
}
