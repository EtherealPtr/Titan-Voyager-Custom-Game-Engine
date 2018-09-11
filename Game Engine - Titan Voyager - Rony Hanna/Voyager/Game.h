#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include "Renderer.h"
#include "Camera.h"
#include "Model.h"
#include "Terrain.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Player.h"
#include "Debugger.h"
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "Font.h"
#include "Framebuffer.h"
#include "Atmosphere.h"
#include "Cloth.h"
#include "Audio.h"

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
	void InitText();
	void InitMultiRenderTarget();
	void InitCloth();
	void InitPlayer();
	void InitAudio();
	void ProcessInput(std::vector<SDL_Event>&);
	void GameLoop();
	void UpdateGame();
	void UpdateMenu();
	void RenderScene();
	void RestartGame();
	void FreezeMouseCursor();
	void FreeMouseCursor();
	void HideIndicator();

private:
	enum { CAM_FORWARD, CAM_BACKWARD, CAM_LEFT, CAM_RIGHT, CAM_RISE, CAM_FALL, TOTAL_CAM_MOVES };
	bool m_bCamMovements[TOTAL_CAM_MOVES];
	
	enum { SATURN_RINGS = 1, SPACE_CUBE, SKYBOX, SATURN, FPS_CROSSHAIR, AMMO, HEALTH, MUZZLE_FLASH, SNIPER_SCOPE, POSTPROCESSING_QUAD, ENEMY_DRONE, 
		   ENEMY_BLAST, MAIN_MENU, INDICATOR, ABOUT, PLAYER_DEATH_SCREEN, PLAYER_VICTORY_SCREEN };
	
	enum class GameState { MAIN_MENU = 100, PLAY, ABOUT, EXIT, TOTAL_STATES };
	GameState m_gameState;
	float m_deltaTime;

private:
	Model m_asteroid, m_flagPole, m_mountainRock;
	std::vector<Model> m_mountainRocks;
	Atmosphere m_atmosphere;
	Terrain m_terrain;
	DirectionalLight m_dirLight;
	PointLight m_pointLight;
	Debugger m_debugger;
	Camera m_camera, m_cameraHUD;
	Framebuffer m_framebuffer;
	Cloth m_flag;
	std::vector<Text> m_texts;
	std::vector<Enemy*> m_enemies;

private:
	int m_mouseX, m_mouseY;
	int m_enemyCount;
	float m_dataTransmitTimer, m_enemySpawnTimer, m_gameStateTimer;
};

#endif // !__GAME_H__