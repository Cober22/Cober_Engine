#pragma once

#include "Cober/Events/EventManager.h"
#include "Cober/Events/Input.h"
#include "Cober/Events/Codes/KeyCodes.h"
#include "Cober/Events/MouseEvents.h"

#include "Cober/Core/Timestep.h"
#include "Cober/Core/Application.h"
#include "Cober/Core/Window.h"

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	class CameraController
	{
	public:
		CameraController() {};
		void LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 upAxis);
		void RecalculateMatrix(); 

		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& event) = 0;
		virtual void Resize(float width, float heigth) = 0;

	public:
		void SetProjection(float left, float right, float bottom, float top);
		void SetProjection(float angle, glm::vec2 aspectRatio, float nearPlane, float farPlane);

		virtual bool OnMouseMotion(MouseMovedEvent& event) = 0;
		virtual bool OnMouseScrolled(MouseScrolledEvent& event) = 0;
		virtual bool OnWindowResized(WindowResizeEvent& event) = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetModelMatrix() const = 0;
	protected:
		// Camera Matrix
		glm::mat4 c_VMatrix;
		glm::mat4 c_PMatrix;
		glm::mat4 c_MMatrix;

		//Camera Attributes
		glm::vec3 c_position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 c_direction = { 0.0f, 0.0f, -1.0f };	// Pointing to the Z Axis
		glm::vec2 c_aspectRatio = { 800.0f, 600.0f };

		// Camera Axis
		glm::vec3 c_rightAxis = glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, c_direction));
		glm::vec3 c_upAxis = { 0.0f, 1.0f, 0.0f };//glm::cross(c_direction, c_rightAxis);
		glm::vec3 c_eye = {0.0f, 0.0f, 0.0f};
	protected:	// Orthographic
		float c_zoomLevel = 5.0f;
		float c_OrthoRotation = 0.0f;
		float c_OrthoTraslationSpeed = 1.0f, c_OrthoRotationSpeed = 10.0f;

	protected:	// Perspective
		float c_CamSensitivity = 5.0f;
		float c_pitch = 0.0f, c_yaw = -90.0f, c_yawSpeed = 20.0f, c_roll = 0.0f;
		float c_fov = 45.0f, c_nearPlane = 0.1f, c_farPlane = 100.0f;
		float c_PerspTraslationSpeed = c_fov / 5.0f;
		// Mouse Controller
		bool m_firstMouse = true;
		float lastX = c_aspectRatio.x / 2, lastY = c_aspectRatio.y / 2;
		float m_mouseSensitivity = 0.5f;
	};
}