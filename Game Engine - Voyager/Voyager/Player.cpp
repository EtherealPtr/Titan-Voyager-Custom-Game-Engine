#include "Player.h"
#include "Renderer.h"
#include "Utils.h"
#include "Dependencies/SDL2/include/SDL.h"

Player::Player() :
	m_jumpHeight(50.0f),
	m_upwardSpeed(0.0f),
	m_health(100),
	m_isInAir(false)
{}

Player::~Player()
{
	delete m_assaultRifle;
	m_assaultRifle = nullptr;

	delete m_sniperRifle;
	m_sniperRifle = nullptr;
}

void Player::Init(Camera& cam)
{
	m_assaultRifle = new Weapon();
	m_assaultRifle->Init("res/Models3D/Sci-fi_AssaultRifle/AssaultRifle.dae", cam, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs");
	m_assaultRifle->Configure(35, 0.12f, 1.0f);

	m_sniperRifle = new Weapon();
	m_sniperRifle->Init("res/Models3D/Sci-fi_SniperRifle/SniperRifle.obj", cam, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs");
	m_sniperRifle->Configure(7, 1.0f, 1.5f);

	m_currWeapon = m_assaultRifle;
	m_usingAR = true;
}

void Player::Update(Camera& cam, Terrain& terrain, float dt, std::vector<SDL_Event> events)
{
	if (!IsPlayerWalking())
	{
		m_walking = false;
		m_sprinting = false;
	}

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

	// Check if the player is not firing or reloading
	if (!m_firing && !m_reloading)
	{
		// Proceed to playing one of the following animations: Walking - Sprinting - Idle
		if (m_walking)
		{
			m_currWeapon->GetAnimComponent().PlayWalkFPS(m_currWeapon->GetModel(), cam, dt);
		}
		else if (m_sprinting)
		{
			m_currWeapon->GetAnimComponent().PlaySprintFPS(m_currWeapon->GetModel(), cam, dt);
		}
		else 
		{
			m_currWeapon->GetAnimComponent().PlayIdleFPS(m_currWeapon->GetModel(), cam, dt);
		}
	}

	// Check if player is firing
	if (m_firing && !m_reloading)
	{
		m_currWeapon->Fire(m_currWeapon->GetModel(), cam, dt, m_firing, m_reloading);
	}

	// Check if player is reloading
	if (m_reloading)
	{
		m_currWeapon->Reload(m_currWeapon->GetModel(), cam, dt, m_reloading);
	}

	// Check if player is jumping
	if (m_jumping)
	{
		m_upwardSpeed += m_physicsComponent.GetGravity() * dt;
		cam.GetCameraPos().y += m_upwardSpeed * dt;

		if (cam.GetCameraPos().y < terrain.GetHeightOfTerrain(cam.GetCameraPos().x, cam.GetCameraPos().z) + 10.0f)
		{
			m_upwardSpeed = 0.0f;
			cam.GetCameraPos().y = terrain.GetHeightOfTerrain(cam.GetCameraPos().x, cam.GetCameraPos().z) + 10.0f;
			m_jumping = false;
		}
	}
	else
	{
		cam.GetCameraPos().y = terrain.GetHeightOfTerrain(cam.GetCameraPos().x, cam.GetCameraPos().z) + 10.0f;
		m_isInAir = false;
	}
}

// Function that checks if the camera (FPS character) is moving 
bool Player::IsPlayerWalking()
{
	return m_bCamMovements[CAM_FORWARD] || m_bCamMovements[CAM_BACKWARD] || m_bCamMovements[CAM_LEFT] || m_bCamMovements[CAM_RIGHT];
}

void Player::Switch()
{
	if (m_usingAR)
	{
		m_usingAR = false;
		m_sniperRifle->GetAnimComponent().SetWeaponZOffset(-4.0f);
		m_sniperRifle->GetAnimComponent().SetSprintWeaponZOffset(-2.5f);
		m_currWeapon = m_sniperRifle;
	}
	else
	{
		m_usingAR = true;
		m_assaultRifle->GetAnimComponent().SetWeaponZOffset(-2.5f);
		m_assaultRifle->GetAnimComponent().SetSprintWeaponZOffset(-2.5f);
		m_currWeapon = m_assaultRifle;
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
				//m_bCamMovements[CAM_RISE] = true;
				break;

			case SDLK_f:
				//m_bCamMovements[CAM_FALL] = true;
				break;

			case SDLK_SPACE:
				if (!m_isInAir)
				{
					m_jumping = true;
					m_isInAir = true;
					m_upwardSpeed = m_jumpHeight;
				}

				break;

			case SDLK_LSHIFT:
				// Only sprint if the player is already walking 
				if (IsPlayerWalking())
				{
					cam.SetCameraSpeed(30.0f);
					m_sprinting = true;
				}

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
				break;

			case SDLK_s:
				m_bCamMovements[CAM_BACKWARD] = false;
				break;

			case SDLK_a:
				m_bCamMovements[CAM_LEFT] = false;
				break;

			case SDLK_d:
				m_bCamMovements[CAM_RIGHT] = false;
				break;

			case SDLK_r:
				//m_bCamMovements[CAM_RISE] = false;
				m_reloading = true;
				break;

			case SDLK_f:
				//m_bCamMovements[CAM_FALL] = false;
				break;

			case SDLK_q:
				Switch();
				break;

			case SDLK_LSHIFT:
				m_sprinting = false;

				if (IsPlayerWalking())
				{
					m_walking = true;
				}

				cam.SetCameraSpeed(20.0f);

				break;

			default: break;
			}

			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			switch (i->button.button)
			{
			case SDL_BUTTON_LEFT:
				m_firing = true;
				break;

			default: break;
			}

			break;
		}

		case SDL_MOUSEBUTTONUP:
		{
			switch (i->button.button)
			{
			case SDL_BUTTON_LEFT:
				m_firing = false;
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