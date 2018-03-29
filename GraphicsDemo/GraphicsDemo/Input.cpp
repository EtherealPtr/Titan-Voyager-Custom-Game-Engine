#include "Input.h"

unsigned short int Input::ProcessInput()
{
	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
		case SDL_QUIT:
			m_processedInput = (int)GameState::EXIT;
			return m_processedInput;
			break;

			// KEYBOARD_INPUT
			case SDL_KEYDOWN:
			{
				switch (m_event.key.keysym.sym)
				{
				case SDLK_z:
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
