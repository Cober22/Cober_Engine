#pragma once

#include "Cober/Timestep.h"

#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, glm::vec2 aspectRatio, float nearPlane, float farPlane);

		void OnUpdate(Timestep ts);
		void OnEvent(SDL_Event& event);

		void SetProjection(float angle, glm::vec2 aspectRatio, float nearPlane, float farPlane);

		void LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 upAxis);

		const glm::mat4& GetProjectionMatrix() const { return c_PMatrix; }
		const glm::mat4& GetViewMatrix() const { return c_VMatrix; }
		const glm::mat4& GetModelMatrix() const { return c_MMatrix; }
		const glm::mat4& GetMVPMatrix() const { return c_MVPMatrix; }

		void SetProjectionMatrix(glm::mat4 projectionMatrix) { c_PMatrix = projectionMatrix;  RecalculateMVPMatrix(); }
		void SetViewMatrix(glm::mat4 viewMatrix) { c_VMatrix = viewMatrix;  RecalculateMVPMatrix(); }
		void SetModelMatrix(glm::mat4 modelMatrix) { c_MMatrix = modelMatrix;  RecalculateMVPMatrix(); }
	private:
		void RecalculateMVPMatrix();
	private:
		void OnMouseMotion(SDL_MouseMotionEvent& e);
		void OnMouseScrolled(SDL_MouseWheelEvent& e);
		void OnWindowResized(SDL_WindowEvent& e);
	private:
		// Camera Attributes
		glm::mat4 c_PMatrix;
		glm::mat4 c_VMatrix;
		glm::mat4 c_MMatrix;
		glm::mat4 c_MVPMatrix;

		float c_CamSensitivity = 5.0f;
		float c_pitch = 0.0f, c_yaw = -90.0f, c_yawSpeed = 10.0f, c_roll = 0.0f;
		glm::vec2 c_aspectRatio = { 800.0f, 600.0f };
		float c_fov = 45, c_nearPlane = 0.1f, c_farPlane = 100.0f;
		float c_traslationSpeed = c_fov / 10;

		// Camera Position & Rotation		
		glm::vec3 c_position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 c_direction = { 0.0f, 0.0f, -1.0f };	// Pointing to the Z Axis
		// Camera Axis
		glm::vec3 c_rightAxis = glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, c_direction));
		glm::vec3 c_upAxis = glm::cross(c_direction, c_rightAxis);

		// Mouse Controller
		bool m_firstMouse = true;
		float lastX = c_aspectRatio.x/2, lastY = c_aspectRatio.y/2;
		float m_mouseSensitivity = 0.1f;
	};
}