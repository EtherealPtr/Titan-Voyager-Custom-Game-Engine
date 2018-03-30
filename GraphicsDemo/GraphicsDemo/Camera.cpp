#include "Camera.h"
#include "Dependencies/SDL2/include/SDL.h"

Camera::Camera() :
	m_cameraPos(glm::vec3(0.0f, 0.0f, 1.0f)),
	m_cameraForward(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_cameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f))
{
	m_cameraRight = glm::normalize(glm::cross(m_cameraUpVector, -m_cameraForward));
}

Camera::~Camera()
{
}

void Camera::InitCameraPerspective(float fov, float aspectRatio, float near, float far)
{
	m_projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, up, near, far);
}

void Camera::Update()
{
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraForward, m_cameraUpVector);
}