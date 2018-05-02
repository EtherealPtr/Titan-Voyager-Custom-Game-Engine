#include "Enemy.h"
#include "Renderer.h"

Enemy::Enemy(Camera& cam) :
	m_pos(glm::vec3(50.0f, 0.0f, 0.0f)),
	m_maximumSpeed(15.0f),
	m_velocity(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_camera(cam)
{}

Enemy::~Enemy()
{}

void Enemy::InitMesh()
{
	m_model.Init("res/Models3D/Rifle/M24_R_Low_Poly_Version_obj.obj", m_camera, "res/Shaders/SingleModelLoader.vs", "res/Shaders/SingleModelLoader.fs", false);
}

void Enemy::Draw()
{
	m_model.Draw(m_camera, m_pos);
}

void Enemy::Update(Terrain& terrain, Camera& cam)
{
	m_camera = cam;
	m_pos.y = terrain.GetHeightOfTerrain(m_pos.x, m_pos.z) + 5.0f;
}

void Enemy::SetPos(glm::vec3& pos)
{
	m_pos = pos;
}

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