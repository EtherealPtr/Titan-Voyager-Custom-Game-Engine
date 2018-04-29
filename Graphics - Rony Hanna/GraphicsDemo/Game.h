#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include "Renderer.h"
#include "Camera.h"
#include "Model.h"
#include "Terrain.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Player.h"
#include "Physics.h"
#include "Debugger.h"
#include "Enemy.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void InitMeshes();
	void InitLights();
	void InitDebugger();
	void ProcessInput(std::vector<SDL_Event>&);
	void GameLoop();
	void Update();


private:
	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];
	
	enum { SATURN_RINGS = 1, SPACE_CUBE, SKYBOX, SATURN, MARS, MERCURY, NEPTUNE, EARTH, FPS_CROSSHAIR, ENEMY01 };
	
	enum class GameState { MAIN_MENU = 100, PLAY, EXIT, TOTAL_STATES };
	GameState m_gameState;
	float m_deltaTime;

private:
	Model m_asteroid;
	Model m_aircraft;
	Model m_weapon;
	Terrain m_terrain;
	DirectionalLight m_dirLight;
	PointLight m_pointLight;
	SpotLight m_spotlight;
	Player m_player;
	Physics m_physics;
	Debugger m_debugger;
	Camera m_camera;
	Camera m_cameraHUD;
	std::vector<Enemy*> m_enemies;

private:
	bool m_spaceScene;
};

#endif // !__GAME_H__