#include "Enemy.h"
#include "Renderer.h"
#include "Utils.h"
#include <cmath>
#include "Player.h"

Enemy::Enemy(Camera& cam) :
	m_pos(Utils::GetInstance().RandomNumBetweenTwo(50.0f, 450.0f), 0.0f, Utils::GetInstance().RandomNumBetweenTwo(0.0f, 450.0f)),
	m_maximumSpeed(15.0f),
	m_velocity(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_camera(cam),
	m_health(100),
	m_dead(false),
	m_distance(0.0f),
	m_attackDistance(125.0f),
	m_takingDamage(false),
	m_evade(false),
	m_damageTakenDuration(0.0f),
	m_evadeDurationCounter(0.0f),
	m_evadeRight(false)
{
	m_particleEffect.Init("res/Shaders/Particle System Shaders/VertexShader.vs",
						  "res/Shaders/Particle System Shaders/GeometryShader.geom",
						  "res/Shaders/Particle System Shaders/FragmentShader.fs", 20, "redOrb");
}

Enemy::~Enemy()
{}

void Enemy::Draw(short int id)
{
	if (!m_dead)
	{
		Renderer::GetInstance().GetComponent(id).SetTransform(m_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		m_particleEffect.Render(m_camera, m_deltaTime, glm::vec3(m_pos.x - 1.7f, m_pos.y + 4.5f, m_pos.z - 0.4f)); 
		Renderer::GetInstance().GetComponent(id).Draw(m_camera, glm::vec3(0.0f, 0.0f, 0.0f), false, Player::GetInstance().GetSpotLight());
	}
}

void Enemy::Update(Terrain& terrain, Camera& cam, float dt)
{
	// Check if this enemy is dead, otherwise update its properties
	if (m_health <= 0)
		m_dead = true;
	else
	{
		m_deltaTime = dt;
		m_camera = cam;
		m_pos.y = terrain.GetHeightOfTerrain(m_pos.x, m_pos.z) + 5.0f;
		m_distance = CalcDistance(m_pos, cam.GetCameraPos());

		// Check if enemy is within attack distance range
		if (m_distance < m_attackDistance)
		{
			// Fire on the player 

			// Check if the player is getting too close
			if (m_distance < 75.0f)
			{
				Flee(cam, dt);
			}
		}
		else
		{
			Seek(cam, dt);
		}

		// Check if enemy is being hit
		if (m_takingDamage)
		{
			// Create damage taken duration window to simulate enemy panick behaviour
			m_damageTakenDuration += 0.1f * m_deltaTime;

			// Check if the damage taken duration window has exceeded a small threshold
			if (m_damageTakenDuration > 0.02f)
			{
				// Stop panicking
				m_takingDamage = false;
				m_damageTakenDuration = 0.0f;
			}
			else
			{
				// Generate random number (1 - 100)
				int dashSidewaysChance = Utils::GetInstance().RandomNumBetween1And100();

				// Check the chance for evade (3% chance)
				if (dashSidewaysChance > 97)
				{
					// Mark the next goal of the enemy (to evade) 
					m_evade = true;

					// Generate random side the enemy should evade towards
					int dashSide = Utils::GetInstance().RandomNumBetween1And100();

					// 50/50 chance to evade in either sides
					if (dashSide > 50)
						m_evadeRight = false;
					else
						m_evadeRight = true;

					// Stop panicking
					m_takingDamage = false;
					m_damageTakenDuration = 0.0f;
				}
			}
		}

		if (m_evade)
		{
			if (!m_evadeRight)
			{
				m_pos.x -= (m_maximumSpeed * 5) * m_deltaTime;
			}
			else
			{
				m_pos.x += (m_maximumSpeed * 5) * m_deltaTime;
			}

			m_evadeDurationCounter += 0.1f * m_deltaTime;

			if (m_evadeDurationCounter > 0.07f)
			{
				m_evade = false;
				m_evadeRight = false;
				m_evadeDurationCounter = 0.0f;
			}
		}
	}
}

void Enemy::ReduceHealth(int amount)
{
	m_health -= amount;
	m_takingDamage = true;
}

inline float Enemy::CalcDistance(glm::vec3& enemyPos, glm::vec3& playerPos)
{
	return sqrt(powf(enemyPos.x - playerPos.x, 2.0f) + powf(enemyPos.y - playerPos.y, 2.0f) + powf(enemyPos.z - playerPos.z, 2.0f));
}

void Enemy::Seek(Camera& target, float dt)
{
	glm::vec3 desiredVelocity = target.GetCameraPos() - m_pos;
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= m_maximumSpeed;

	glm::vec3 steering = desiredVelocity - m_velocity;
	steering = glm::clamp(steering, -m_maximumSpeed, m_maximumSpeed);

	m_pos += steering * dt;
}

void Enemy::Flee(Camera& target, float dt)
{
	glm::vec3 desiredVelocity = target.GetCameraPos() - m_pos;
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= m_maximumSpeed;

	glm::vec3 steering = desiredVelocity - m_velocity;
	steering = glm::clamp(steering, -m_maximumSpeed, m_maximumSpeed);

	m_pos -= steering * dt;
}