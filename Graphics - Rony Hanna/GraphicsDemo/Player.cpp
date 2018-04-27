#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(Model& weapon, Camera& cam, float dt, std::vector<SDL_Event> events)
{
	ProcessInput(cam, dt, events);

#pragma region // CAMERA_MOVEMENT
	if (m_bCamMovements[CAM_FORWARD])
		cam.MoveForward(dt);
	if (m_bCamMovements[CAM_BACKWARD])
		cam.MoveBackward(dt);
	if (m_bCamMovements[CAM_LEFT])
		cam.StrafeLeft(dt);
	if (m_bCamMovements[CAM_RIGHT])
		cam.StrafeRight(dt);
	if (m_bCamMovements[CAM_RISE])
		cam.Rise(dt);
	if (m_bCamMovements[CAM_FALL])
		cam.Fall(dt);
#pragma endregion

	if (m_walking)
	{
		m_animationComponent.PlayWalkFPS(weapon, cam, dt);
	}
	else if (m_sprinting)
	{
		m_animationComponent.PlaySprintFPS(weapon, cam, dt);
	}
	else
	{
		m_animationComponent.PlayIdleFPS(weapon, cam, dt);
	}
}

void Player::ProcessInput(Camera& cam, float dt, std::vector<SDL_Event> events)
{
	for (auto i = events.begin(); i != events.end(); ++i)
	{
		switch (i->type)
		{
			// KEYBOARD_INPUT
		case SDL_KEYDOWN:
		{
			switch (i->key.keysym.sym)
			{
			case SDLK_w:
				m_bCamMovements[CAM_FORWARD] = true;
				m_walking = true;
				break;

			case SDLK_s:
				m_bCamMovements[CAM_BACKWARD] = true;
				m_walking = true;
				break;

			case SDLK_a:
				m_bCamMovements[CAM_LEFT] = true;
				m_walking = true;
				break;

			case SDLK_d:
				m_bCamMovements[CAM_RIGHT] = true;
				m_walking = true;
				break;

			case SDLK_r:
				m_bCamMovements[CAM_RISE] = true;
				break;

			case SDLK_f:
				m_bCamMovements[CAM_FALL] = true;
				break;

			case SDLK_LSHIFT:
				cam.SetCameraSpeed(30.0f);
				m_sprinting = true;
				m_walking = false;
				break;

			default: break;
			}

			break;
		}

		case SDL_KEYUP:
		{
			switch (i->key.keysym.sym)
			{
			case SDLK_w:
				m_bCamMovements[CAM_FORWARD] = false;
				m_walking = false;
				break;

			case SDLK_s:
				m_bCamMovements[CAM_BACKWARD] = false;
				m_walking = false;
				break;

			case SDLK_a:
				m_bCamMovements[CAM_LEFT] = false;
				m_walking = false;
				break;

			case SDLK_d:
				m_bCamMovements[CAM_RIGHT] = false;
				m_walking = false;
				break;

			case SDLK_r:
				m_bCamMovements[CAM_RISE] = false;
				break;

			case SDLK_f:
				m_bCamMovements[CAM_FALL] = false;
				break;

			case SDLK_LSHIFT:
				m_sprinting = false;

				if (m_bCamMovements[CAM_FORWARD] || m_bCamMovements[CAM_BACKWARD] || m_bCamMovements[CAM_LEFT] || m_bCamMovements[CAM_RIGHT])
				{
					m_walking = true;
				}

				cam.SetCameraSpeed(20.0f);

				break;

			default: break;
			}

			break;
		}
		// KEYBOARD_INPUT END

		default:
			break;
		}
	}
}