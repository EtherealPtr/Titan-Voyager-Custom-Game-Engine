#include "Enemy.h"
#include "Renderer.h"
#include "Utils.h"
#include <cmath>
#include "Player.h"
#include "Audio.h"

Enemy::Enemy(Camera& cam) :
	m_pos(Utils::GetInstance().RandomNumBetweenTwo(50.0f, 450.0f), 0.0f, Utils::GetInstance().RandomNumBetweenTwo(0.0f, 450.0f)),
	m_maximumSpeed(15.0f),
	m_maximumDroneSpeed(100.0f),
	m_velocity(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_camera(cam),
	m_health(100),
	m_blastRadius(0.01f),
	m_distance(0.0f),
	m_shootDuration(0.0f),
	m_evadeDurationCounter(0.0f),
	m_damageTakenDuration(0.0f),
	m_attackDamage(10.0f),
	m_currLifeTimer(0.0f),
	m_dead(false),
	m_takingDamage(false),
	m_evade(false),
	m_evadeRight(false),
	m_fire(false),
	m_droneStatus(true),
	m_damageToken(true),
	m_canRespawn(true),
	m_dronePos(m_pos)
{
	m_particleEffect.Init("res/Shaders/Particle System Shaders/VertexShader.vs",
						  "res/Shaders/Particle System Shaders/GeometryShader.geom",
						  "res/Shaders/Particle System Shaders/FragmentShader.fs", 20, "redOrb");
}

Enemy::~Enemy()
{}

void Enemy::Draw(short int enemyId, short int enemyDroneId)
{
	if (!m_dead)
	{
		// Activate enemy's shader program
		Renderer::GetInstance().GetComponent(enemyId).GetShaderComponent().ActivateProgram();

		// Check if the enemy is taking damage, if so, make the enemy flash red
		if (m_takingDamage)
			Renderer::GetInstance().GetComponent(enemyId).GetShaderComponent().SetBool("damaged", true);
		else
			Renderer::GetInstance().GetComponent(enemyId).GetShaderComponent().SetBool("damaged", false);

		// Update the enemy's transform and particle system every frame and draw the enemy
		Renderer::GetInstance().GetComponent(enemyId).SetTransform(m_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		Renderer::GetInstance().GetComponent(enemyId).Draw(m_camera, glm::vec3(0.0f, 0.0f, 0.0f), false, Player::GetInstance().GetSpotLight());

		if (m_currLifeTimer >= 0.2f)
			m_particleEffect.Render(m_camera, m_deltaTime, glm::vec3(m_pos.x - 1.7f, m_pos.y + 4.5f, m_pos.z - 0.4f));

		// Check if a small drone has been fired by the enemy
		if (m_droneActive)
		{
			// Update the small drone's transform per frame
			Renderer::GetInstance().GetComponent(enemyDroneId).SetTransform(m_dronePos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f));
			Renderer::GetInstance().GetComponent(enemyDroneId).Draw(m_camera);

			// Check if the player is colliding with the drone
			if (Physics::GetInstance().PointInSphere(m_camera, m_dronePos, 2.0f))
			{
				// Inflict damage on player
				Physics::GetInstance().OnPlayerHit(m_attackDamage);
			}
		}
	}
	else
	{
		m_currLifeTimer = 0.0f;
		Respawn();
	}
}

void Enemy::DrawShockwave(short int enemyDroneBlastId)
{
	// Check if the smart drone has reached its desired destination where it is about to explode
	if (m_droneSelfDestruct)
	{
		// Increase the explosion radius
		m_blastRadius += 5.0f * m_deltaTime;

		if (m_blastRadius < 7.0f)
		{
			// Update explosion blast 
			Renderer::GetInstance().GetComponent(enemyDroneBlastId).SetTransform(m_oldPlayerPos, glm::vec3(m_blastRadius * 20, m_blastRadius * 20, m_blastRadius * 20), glm::vec3(m_blastRadius));
			Renderer::GetInstance().GetComponent(enemyDroneBlastId).Draw(m_camera);

			// Check if enemy can damage and player is caught within the growing blast
			if (m_damageToken && Physics::GetInstance().PointInSphere(m_camera, m_oldPlayerPos, (m_blastRadius * 4)))
			{
				// Inflict damage on player
				Physics::GetInstance().OnPlayerHit(m_attackDamage);

				// Take away the enemy's damage token (so that player does not continuously get hit while within the blast) 
				m_damageToken = false;
			}
		}
		else
		{
			// Restart blast properties
			m_droneSelfDestruct = false;
			m_blastRadius = 0.01f;

			// Restore the enemy's damage token
			m_damageToken = true;
		}
	}
}

void Enemy::Update(Terrain& terrain, Camera& cam, float dt)
{
	if (!m_dead)
	{
		m_deltaTime = dt;
		m_camera = cam;
		m_pos.y = terrain.GetHeightOfTerrain(m_pos.x, m_pos.z) + 5.0f;
		m_distance = CalcDistance(m_pos, cam.GetCameraPos());
		m_currLifeTimer += 0.1f * dt;

		// Check if the player is getting too close, if so then run away, if not then move towards player
		if (m_distance < 75.0f)
		{
			Flee(cam, dt);
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

		if (m_shootDuration > 1.0f)
		{
			m_shootDuration = 0.0f;

			// Check if the enemy unit is not firing already
			if (!m_fire)
				m_fire = true;
		}
		else
		{
			m_shootDuration += Utils::GetInstance().RandomNumBetweenTwo(0.1f, 0.5f) * dt;
		}

		if (m_fire)
		{
			m_dronePos.y = terrain.GetHeightOfTerrain(m_dronePos.x, m_dronePos.z) + 10.0f;
			Fire(cam, terrain, dt);
		}
	}
	else
	{
		m_pos.y = -999.0f;
		m_dronePos.y = -999.0f;
	}
}

void Enemy::ReduceHealth(int amount)
{
	m_health -= amount;
	m_takingDamage = true;

	if (m_health <= 0)
	{
		m_dead = true;

		// Player one of the monster dead sounds
		if (Utils::GetInstance().RandomNumBetweenTwo(1.0f, 2.0f) > 1.5f)
			Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("EnemyDead")->second);
		else
			Audio::GetInstance().PlaySound(Audio::GetInstance().GetSoundsMap().find("EnemyDead2")->second);
	}
}

// Function that finds the distance between two vectors
inline float Enemy::CalcDistance(glm::vec3& enemyPos, glm::vec3& playerPos)
{
	return sqrt(powf(enemyPos.x - playerPos.x, 2.0f) + powf(enemyPos.y - playerPos.y, 2.0f) + powf(enemyPos.z - playerPos.z, 2.0f));
}

// Function that finds the distance between two vectors without taking the y-axis into consideration (XZ plane only)
inline float Enemy::CalcDistanceNoHeight(glm::vec3& enemyPos, glm::vec3& playerPos)
{
	return sqrt(powf(enemyPos.x - playerPos.x, 2.0f) + powf(enemyPos.z - playerPos.z, 2.0f));
}

void Enemy::SetRespawnStatus(bool canRespawn)
{
	m_canRespawn = canRespawn;
}

void Enemy::Seek(Camera& target, const float dt)
{
	glm::vec3 desiredVelocity = target.GetCameraPos() - m_pos;
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= m_maximumSpeed;

	glm::vec3 steering = desiredVelocity - m_velocity;
	steering = glm::clamp(steering, -m_maximumSpeed, m_maximumSpeed);

	m_pos += steering * dt;
}

void Enemy::Flee(Camera& target, const float dt)
{
	glm::vec3 desiredVelocity = target.GetCameraPos() - m_pos;
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= m_maximumSpeed;

	glm::vec3 steering = desiredVelocity - m_velocity;
	steering = glm::clamp(steering, -m_maximumSpeed, m_maximumSpeed);

	m_pos -= steering * dt;
}

void Enemy::Fire(Camera& target, Terrain& terrain, const float dt)
{
	// Check if the drone has just been fired
	if (m_droneStatus && !m_droneSelfDestruct)
	{
		// Set the drone's position equal to the big sphere enemy position
		m_dronePos = m_pos;

		// Store the old player's position and set the drone active to true for rendering purposes
		m_oldPlayerPos = target.GetCameraPos();
		m_droneActive = true;

		// Set drone's status to false so that the player position is only retrieved once
		m_droneStatus = false;
	}

	// Travel to the old player position 
	glm::vec3 desiredVelocity = m_oldPlayerPos - m_dronePos;
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= m_maximumDroneSpeed;

	glm::vec3 steering = desiredVelocity - m_velocity;
	steering = glm::clamp(steering, -m_maximumDroneSpeed, m_maximumDroneSpeed);

	m_dronePos += steering * dt;

	// Check if the drone reached the old player's position
	float dist = CalcDistanceNoHeight(m_dronePos, m_oldPlayerPos);

	if (dist <= 3)
	{
		// Drone self-destruct 
		m_droneSelfDestruct = true;

		// Recycle the drone for future use
		m_fire = false;
		m_droneStatus = true;
		m_dronePos = m_pos;
		m_dronePos.y = -999.0f;
	}
}

void Enemy::Respawn()
{
	if (m_canRespawn)
	{
		m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_dronePos = m_pos;
		m_respawnTimer += 1.0f * m_deltaTime;

		if (m_respawnTimer >= 15.0f)
		{
			// Restart some properties
			m_respawnTimer = 0.0f;
			m_currLifeTimer = 0.0f;
			m_blastRadius = 0.01f;
			m_shootDuration = 0.0f;
			m_evadeDurationCounter = 0.0f; 
			m_damageTakenDuration = 0.0f;
			m_takingDamage = false;
			m_dead = false;
			m_droneSelfDestruct = false;
			m_blastRadius = 0.01f;
			m_health = 100;

			// Set new spawn position
			m_pos = glm::vec3(Utils::GetInstance().RandomNumBetweenTwo(50.0f, 520.0f), 0.0f, Utils::GetInstance().RandomNumBetweenTwo(0.0f, 650.0f));
		}
	}
}