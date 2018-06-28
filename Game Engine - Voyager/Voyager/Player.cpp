#include "Player.h"
#include "Renderer.h"
#include "Utils.h"

Player::Player() :
	m_ammoCount(35),
	m_fireTimer(0.0f),
	m_fireRate(0.12f),
	m_jumpHeight(50.0f),
	m_upwardSpeed(0.0f),
	m_health(100),
	m_isInAir(false)
{}

Player::~Player()
{}

void Player::Update(Model& weapon, Camera& cam, Terrain& terrain, float dt, std::vector<SDL_Event> events)
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

	// Check if player is firing
	if (m_firing && !m_reloading)
	{
		Fire(weapon, cam, dt);

		if (m_fireTimer < 0.02f)
		{
			// Play muzzle flash effect (render textured quad in eye space in front of weapon)
			glm::mat4 model(1.0f);
			glm::mat4 translation = glm::translate(glm::vec3(0.9f, -1.4f, -6.5f));
			glm::mat4 rotation = glm::rotate(Utils::GetInstance().RandomNumBetweenTwo(1.0f, 360.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 scaleMat = glm::scale(glm::vec3(Utils::GetInstance().RandomNumBetweenTwo(2.0f, 2.5f), Utils::GetInstance().RandomNumBetweenTwo(2.0f, 2.5f), 1.0f));
			glm::mat4 invViewMat = glm::inverse(cam.GetViewMatrix());
			model = invViewMat * translation * rotation * scaleMat;
			Renderer::GetInstance().GetComponent(12).Draw(model, cam, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	// Check if player is reloading
	if (m_reloading)
	{
		Reload(weapon, cam, dt);
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

void Player::Fire(Model& weapon, Camera& cam, float dt)
{
	m_fireTimer += dt;
	m_animationComponent.PlayIdleFPS(weapon, cam, dt);

	if (m_fireTimer > m_fireRate)
	{
		m_animationComponent.PlayFireFPS(weapon, cam, dt);

		--m_ammoCount;

		if (m_ammoCount <= 0)
		{
			m_ammoCount = 0;
			m_firing = false;
			m_reloading = true;
		}

		m_fireTimer = 0.0f;
	}
	else
	{
		m_muzzleFlash = false; 
	}
}

void Player::Reload(Model& weapon, Camera& cam, float dt)
{
	m_animationComponent.PlayReloadFPS(weapon, cam, dt);
	m_reloadTimer += 0.4f * dt;
 
	if (m_reloadTimer >= 1.0f)
	{
		m_ammoCount = 35;
		m_reloadTimer = 0.0f;
		m_reloading = false;
	}
}

// Function that checks if the camera (FPS character) is moving 
bool Player::IsPlayerWalking()
{
	return m_bCamMovements[CAM_FORWARD] || m_bCamMovements[CAM_BACKWARD] || m_bCamMovements[CAM_LEFT] || m_bCamMovements[CAM_RIGHT];
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
				m_bCamMovements[CAM_RISE] = false;
				m_reloading = true;
				break;

			case SDLK_f:
				m_bCamMovements[CAM_FALL] = false;
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