#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Dependencies/SDL2/include/SDL.h"
#include "GameObject.h"
#include <vector>
#include "Shape.h"

enum { TRIANGLE, QUAD, CUBE, SPHERE, TOTAL_SHAPES };

class Renderer
{
public:
	~Renderer();

	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	int GetWindowWidth()  { return m_screenWidth; }
	int GetWindowHeight() { return m_screenHeight; }

	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;

	void Run();
	void RenderObjects(Camera cam);
	void InitMesh(GLuint meshType, char* texId, int objId, std::vector<char*> shaders, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, char* normalMapping = "blank", bool instancing = false);
	SDL_Window* GetAppWindow() { return window; }

	GameObject& GetComponent(char* textureId);
	GameObject& GetComponent(int objId);

private:
	void SetWindowAttribs();
	bool InitSDLAndOpenGL();
	SDL_Window* window;
	SDL_GLContext m_openGlContext;
	int m_screenWidth, m_screenHeight;

private:
	Renderer() {};
	Shape* m_shape;
	std::vector<Shape*> shapes;
	std::vector<GameObject*> m_gameEntities;
	Camera m_camera;
};

#endif // !__RENDERER_H__