#pragma once
#ifndef __INPUT_H__
#define __INPUT_H__

#include "Dependencies/glew/include/GL/glew.h"
#include "Dependencies/SDL2/include/SDL.h"

enum class GameState { MAIN_MENU, PLAY, EXIT, TOTAL_STATES };
enum class Controller {W_DOWN = 50, S_DOWN, A_DOWN, D_DOWN, W_UP, S_UP, A_UP, D_UP };

class Input
{
public:
	using uShort = unsigned short int;

	uShort ProcessInput();

private:
	SDL_Event m_event;
	SDL_KeyboardEvent* m_pkey;
};

#endif // !__INPUT_H__