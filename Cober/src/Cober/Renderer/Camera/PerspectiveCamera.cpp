#include "pch.h"
#include "PerspectiveCamera.h"

namespace Cober {

	PerspectiveCamera::PerspectiveCamera(float fov, glm::vec2 aspectRatio, float nearPlane, float farPlane)
	{
		c_VMatrix = glm::mat4(1.0f);
		c_MMatrix = glm::mat4(1.0f);

		c_PMatrix = glm::perspective(glm::radians(fov), aspectRatio.x / aspectRatio.y, nearPlane, farPlane);
		c_fov = fov;
		c_aspectRatio = aspectRatio;
		c_nearPlane = nearPlane;
		c_farPlane = farPlane;
	}

	void PerspectiveCamera::OnUpdate(Timestep ts) {

		// --- Keyboard keys
		if (Input::IsKeyPressed(KEY_A))
			c_position -= c_rightAxis * c_PerspTraslationSpeed * glm::vec3(ts);
		if (Input::IsKeyPressed(KEY_D))
			c_position += c_rightAxis * c_PerspTraslationSpeed * glm::vec3(ts);
		if (Input::IsKeyPressed(KEY_S))
			c_position -= c_direction * c_PerspTraslationSpeed * glm::vec3(ts);
		if (Input::IsKeyPressed(KEY_W))
			c_position += c_direction * c_PerspTraslationSpeed * glm::vec3(ts);
		
		if (Input::IsKeyPressed(KEY_Q))
			c_yaw -= c_yawSpeed * c_CamSensitivity * ts;
		if (Input::IsKeyPressed(KEY_E))
			c_yaw += c_yawSpeed * c_CamSensitivity * ts;
				
		glm::vec3 front;
		front.x = cos(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		front.y = sin(glm::radians(c_pitch));
		front.z = sin(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		c_direction = glm::normalize(front);
		
		c_position.y = 0.0f;
		RecalculateMatrix();
	}

	void PerspectiveCamera::Resize(float width, float heigth) {

		SetProjection(c_fov, {width, heigth}, c_nearPlane, c_farPlane);
	}

	void PerspectiveCamera::OnEvent(Event& event) {
		
		CB_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(CB_BIND_EVENT(PerspectiveCamera::OnMouseMotion));
		dispatcher.Dispatch<MouseScrolledEvent>(CB_BIND_EVENT(PerspectiveCamera::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CB_BIND_EVENT(PerspectiveCamera::OnWindowResized));
	}

	bool PerspectiveCamera::OnMouseMotion(MouseMovedEvent& event) {

		CB_PROFILE_FUNCTION();

		float mouseX = static_cast<float>(event.GetX());
		float mouseY = static_cast<float>(event.GetY());
		
		c_yaw += mouseX	* m_mouseSensitivity;
		c_pitch -= mouseY * m_mouseSensitivity;
		
		c_pitch = std::max(c_pitch, -60.0f);
		c_pitch = std::min(c_pitch, 60.0f);
		
		glm::vec3 front;
		front.x = cos(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		front.y = sin(glm::radians(c_pitch));
		front.z = sin(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		c_direction = glm::normalize(front);
		
		RecalculateMatrix();
		return false;
	}

	bool PerspectiveCamera::OnMouseScrolled(MouseScrolledEvent& event)
	{
		CB_PROFILE_FUNCTION();

		float zoom = c_fov;
		zoom -= event.GetYOffset();
		zoom = std::max(zoom, 1.0f);
		zoom = std::min(zoom, 45.0f);
		c_PerspTraslationSpeed = zoom / 5;
		SetProjection(zoom, c_aspectRatio, c_nearPlane, c_farPlane);
		return false;
	}

	bool PerspectiveCamera::OnWindowResized(WindowResizeEvent& event)
	{
		CB_PROFILE_FUNCTION();

		SetProjection(c_fov, { event.GetWidth(), event.GetHeight() }, c_nearPlane, c_farPlane);
		return false;
	}
}