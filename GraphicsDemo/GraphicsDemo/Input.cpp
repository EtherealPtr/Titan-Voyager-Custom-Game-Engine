#include "Input.h"

unsigned short int Input::ProcessInput()
{
	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
		case SDL_QUIT:
			return (int)GameState::EXIT;
			break;

			// KEYBOARD_INPUT
			case SDL_KEYDOWN:
			{
				switch (m_event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return (int)GameState::EXIT;
					break;

				case SDLK_z:
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					break;

				case SDLK_w:
					return (int)Controller::W_DOWN;
					break;

				case SDLK_s:
					return (int)Controller::S_DOWN;
					break;

				case SDLK_a:
					return (int)Controller::A_DOWN;
					break;

				case SDLK_d:
					return (int)Controller::D_DOWN;
					break;

				default: break;
				}
			}

			case SDL_KEYUP:
			{
				switch (m_event.key.keysym.sym)
				{
				case SDLK_w:
					return (int)Controller::W_UP;
					break;

				case SDLK_s:
					return (int)Controller::S_UP;
					break;

				case SDLK_a:
					return (int)Controller::A_UP;
					break;

				case SDLK_d:
					return (int)Controller::D_UP;
					break;
				
				default: break;
				}
			}

			// KEYBOARD_INPUT END

			default:
				break;
		}
	}

	return 0;
}
