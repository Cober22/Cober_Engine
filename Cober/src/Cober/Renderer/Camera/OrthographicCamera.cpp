#include "pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {
	
	OrthographicCamera::OrthographicCamera(glm::vec2 aspectRatio)
	{
		c_VMatrix = glm::mat4(1.0f);
		c_MMatrix = glm::mat4(1.0f);
		float ratio = aspectRatio.x / aspectRatio.y;
		c_PMatrix = glm::ortho(-ratio * c_zoomLevel, ratio * c_zoomLevel, -c_zoomLevel, c_zoomLevel, -1.0f, 100.0f);
		
		c_aspectRatio = aspectRatio;
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		c_VMatrix = glm::mat4(1.0f);
		c_MMatrix = glm::mat4(1.0f);
		c_PMatrix = glm::ortho(left, right, bottom, top, -1.0f, 100.0f);
	}

	void OrthographicCamera::OnUpdate(Timestep ts)
	{
		CB_PROFILE_FUNCTION();

		// --- Keyboard keys
		// Wihout Rotation
		if (Input::IsKeyPressed(KEY_A))
			c_position.x -=c_OrthoTraslationSpeed * ts;
		if (Input::IsKeyPressed(KEY_D))
			c_position.x += c_OrthoTraslationSpeed * ts;
		if (Input::IsKeyPressed(KEY_S))
			c_position.y -= c_OrthoTraslationSpeed * ts;
		if (Input::IsKeyPressed(KEY_W))
			c_position.y += c_OrthoTraslationSpeed * ts;
		
		// With Rotation
		/*if (Input::IsKeyPressed(kEY_A)) {
			c_position.x -= cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y -= sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}
		if (Input::IsKeyPressed(KEY_D)) {
			c_position.x += cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y += sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}
		if (Input::IsKeyPressed(KEY_S)]) {
			c_position.x -= -sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y -= cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}
		if (Input::IsKeyPressed(KEY_W)) {
			c_position.x += -sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y += cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}*/
		
		if (Input::IsKeyPressed(KEY_Q))
			c_OrthoRotation -= c_OrthoRotationSpeed * ts;
		if (Input::IsKeyPressed(KEY_E))
			c_OrthoRotation += c_OrthoRotationSpeed * ts;
		
		c_OrthoTraslationSpeed = c_zoomLevel;
		c_OrthoRotationSpeed = c_zoomLevel * 10;
		
		//CONSTRAINS
		c_position.y = std::max(c_position.y, 0.0f);
		c_position.z = 0;
		
		RecalculateMatrix();
	}

	void OrthographicCamera::Resize(float width, float heigth) {

		float ratio = width / heigth;
		SetProjection(-ratio * c_zoomLevel, ratio * c_zoomLevel, -c_zoomLevel, c_zoomLevel);
	}
		
	void OrthographicCamera::OnEvent(Event& event)
	{
		CB_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(CB_BIND_EVENT(OrthographicCamera::OnMouseMotion));
		dispatcher.Dispatch<MouseScrolledEvent>(CB_BIND_EVENT(OrthographicCamera::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CB_BIND_EVENT(OrthographicCamera::OnWindowResized));
	}

	bool OrthographicCamera::OnMouseMotion(MouseMovedEvent& event) {

		float mouseX = static_cast<float>(event.GetX());
		float mouseY = static_cast<float>(event.GetY());
		
		c_yaw -= mouseX * m_mouseSensitivity;
		c_pitch += mouseY * m_mouseSensitivity;
		
		c_pitch = std::max(c_pitch, -2.0f);
		c_pitch = std::min(c_pitch, 2.0f);
		c_yaw = std::max(c_yaw, -91.0f);
		c_yaw = std::min(c_yaw, -89.0f);
		
		glm::vec3 front;
		front.x = cos(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		front.y = sin(glm::radians(c_pitch));
		front.z = sin(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		
		c_direction = glm::normalize(front);
		
		RecalculateMatrix();
		return false;
	}

	bool OrthographicCamera::OnMouseScrolled(MouseScrolledEvent& event) {

		CB_PROFILE_FUNCTION();

		c_zoomLevel -= event.GetYOffset() * 0.25f;
		c_zoomLevel = std::max(c_zoomLevel, 0.25f);
	
		float ratio = c_aspectRatio.x / c_aspectRatio.y;
		SetProjection(-ratio * c_zoomLevel, ratio * c_zoomLevel, -c_zoomLevel, c_zoomLevel);
		return false;
	}

	bool OrthographicCamera::OnWindowResized(WindowResizeEvent& e) {
		
		CB_PROFILE_FUNCTION();

		Resize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}