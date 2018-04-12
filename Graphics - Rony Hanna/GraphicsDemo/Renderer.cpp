#include "Renderer.h"
#include <iostream>

// -------------------
// Author: Rony Hanna
// Description: Destructore that free all allocated memory on the heap
// -------------------
Renderer::~Renderer()
{
	for (auto& gameEntity : m_gameEntities)
	{
		delete gameEntity;
		gameEntity = nullptr;
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
	m_screenWidth = 1440;
	m_screenHeight = 900;

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
	int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	if (ret < 0)
	{
		std::cerr << "ERROR: SDL could not initialize, error: " << SDL_GetError() << "\n";
		return false;
	}

	SetWindowAttribs();

	window = SDL_CreateWindow("Into the Galaxy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL);
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
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);

	glClearColor(0.137255f, 0.137255f, 0.556863f, 1.0f);

	return true;
}

// -------------------
// Author: Rony Hanna
// Description: Function that renders all the game objects in the vector
// -------------------
void Renderer::RenderObjects(Camera cam)
{
	m_camera = cam;
	for (auto& entity : m_gameEntities)
	{
		entity->Draw(m_camera);
	}
}

// -------------------
// Author: Rony Hanna
// Description: Function that creates a game object using a shape factory
// -------------------
void Renderer::InitMesh(GLuint meshType, char* textureId, int objId, char* vs, char* fs, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool bIsInstancing)
{
	m_shape = m_shape->CreateShape(meshType);
	m_shape->InitVertexData();

	GameObject* polygon = new GameObject();
	polygon->CreateGameObj(m_shape->GetVertexData(), m_shape->GetVertexDataCount(), m_shape->GetIndexData(), m_shape->GetIndexDataCount(), bIsInstancing);
	polygon->GetShaderComponent().CreateProgram(vs, fs);

	if (textureId != "skybox")
		polygon->GetTextureComponent().GenerateTexture(textureId);
	else
		polygon->GetTextureComponent().GenerateSkybox();

	polygon->SetTransform(pos, rot, scale);
	polygon->SetTextureId(textureId);
	polygon->SetObjectId(objId);

	delete m_shape;
	m_shape = nullptr;

	m_gameEntities.push_back(polygon);
}

// -------------------
// Author: Rony Hanna
// Description: Function that returns a game object component associated with a given texture id
// -------------------
GameObject& Renderer::GetComponent(char* textureId)
{
	GameObject* resultantObj = nullptr;

	for (auto& obj : m_gameEntities)
	{
		if (obj->GetTextureId() == textureId)
		{
			resultantObj = obj;
			return *resultantObj;
		}
	}

	std::cerr << "ERROR: Unable to find the game object with texture id: " << textureId << ".\n";
	return *resultantObj;
}

// -------------------
// Author: Rony Hanna
// Description: Function that returns a game object component associated with a given numeric id
// -------------------
GameObject& Renderer::GetComponent(int objId)
{
	GameObject* resultantObj = nullptr;

	for (auto& obj : m_gameEntities)
	{
		if (obj->GetObjectId() == objId)
		{
			resultantObj = obj;
			return *resultantObj;
		}
	}

	std::cerr << "ERROR: Unable to find the game object with numeric id: " << objId << ".\n";
	return *resultantObj;
}