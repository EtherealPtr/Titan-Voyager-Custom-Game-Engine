#include "Enemy.h"

Enemy::Enemy() :
	m_pos(glm::vec3(50.0f, 0.0f, 0.0f)),
	m_maximumSpeed(15.0f),
	m_velocity(glm::vec3(1.0f, 1.0f, 1.0f))
{}

Enemy::~Enemy()
{}

void Enemy::Seek(Camera& target, float dt)
{
	glm::vec3 desiredVelocity = target.GetCameraPos() - m_pos;
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= m_maximumSpeed;

	glm::vec3 steering = desiredVelocity - m_velocity;
	steering = glm::clamp(steering, -m_maximumSpeed, m_maximumSpeed);

	m_pos.x += steering.x * dt;
	m_pos.y += steering.y * dt;
	m_pos.z += steering.z * dt;
}
