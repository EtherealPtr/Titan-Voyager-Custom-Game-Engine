#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Dependencies/glm-0.9.9-a2/glm/glm.hpp"
#include "Dependencies/glm-0.9.9-a2/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm-0.9.9-a2/glm/gtc/type_ptr.hpp"
#include "Dependencies/glm-0.9.9-a2/glm/gtx/transform.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	void Update();

	void InitCameraPerspective(float fov, float aspectRatio, float near, float far);
	void InitCameraOrthographic(float left, float right, float bottom, float up, float near, float far);

	void SetCameraPos(glm::vec3 pos)		{ m_cameraPos = pos; }
	void SetCameraView(glm::vec3 view)		{ m_cameraForward = view; }
	void SetCameraUpVector(glm::vec3 up)	{ m_cameraUpVector = up; }

	glm::mat4& GetProjectionMatrix()		{ return m_projection; }
	glm::mat4& GetViewMatrix()				{ return m_view; }
	glm::vec3& GetCameraPos()				{ return m_cameraPos; }
	glm::vec3& GetCameraForward()			{ return m_cameraForward; }
	glm::vec3& GetCameraUpVector()			{ return m_cameraUpVector; }
	glm::vec3& GetCameraRightVector()		{ return m_cameraRight; }

private:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::vec3 m_cameraUpVector;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraForward;
	glm::vec3 m_cameraRight;
};

#endif // !__CAMERA_H__