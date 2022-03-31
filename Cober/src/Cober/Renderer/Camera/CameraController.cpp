#include "pch.h"
#include "CameraController.h"

namespace Cober {
	
	void CameraController::LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 upAxis) {

		c_VMatrix = glm::lookAt(position, target, upAxis) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(c_OrthoRotation), glm::vec3(0, 0, 1));
	}

	void CameraController::RecalculateMatrix() {

		c_rightAxis = glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, c_direction));
		c_upAxis = glm::cross(c_direction, c_rightAxis);
		LookAt(c_position, c_position + c_direction, c_upAxis);
	}

	void CameraController::SetProjection(float left, float right, float bottom, float top) {

		float ratio = c_aspectRatio.x / c_aspectRatio.y;
		c_PMatrix = glm::ortho(-ratio * c_zoomLevel, ratio * c_zoomLevel, -c_zoomLevel, c_zoomLevel, -1.0f, 100.0f);
	}

	void CameraController::SetProjection(float fov, glm::vec2 aspectRatio, float nearPlane, float farPlane) {

		c_PMatrix = glm::perspective(glm::radians(fov), aspectRatio.x / aspectRatio.y, nearPlane, farPlane);
		c_fov = fov;
		c_aspectRatio = aspectRatio;
		c_nearPlane = nearPlane;
		c_farPlane = farPlane;
	}
}