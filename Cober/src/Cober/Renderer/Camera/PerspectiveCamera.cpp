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

		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		
		// --- Keyboard keys
		if (keystate[SDL_SCANCODE_A])
			c_position -= c_rightAxis *c_PerspTraslationSpeed* glm::vec3(ts);
		if (keystate[SDL_SCANCODE_D])
			c_position += c_rightAxis * c_PerspTraslationSpeed * glm::vec3(ts);
		if (keystate[SDL_SCANCODE_S])
			c_position -= c_direction * c_PerspTraslationSpeed * glm::vec3(ts);
		if (keystate[SDL_SCANCODE_W])										
			c_position += c_direction * c_PerspTraslationSpeed * glm::vec3(ts);

		if (keystate[SDL_SCANCODE_Q]) 
			c_yaw -= c_yawSpeed * c_CamSensitivity * ts;
		if (keystate[SDL_SCANCODE_E])
			c_yaw += c_yawSpeed * c_CamSensitivity * ts;

		glm::vec3 front;
		front.x = cos(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		front.y = sin(glm::radians(c_pitch));
		front.z = sin(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		c_direction = glm::normalize(front);

		c_position.y = 0.0f;
		RecalculateMatrix();
	}

	void PerspectiveCamera::OnEvent(SDL_Event& event) {
		
		switch (event.type) {
			case SDL_MOUSEMOTION:
				OnMouseMotion(event.motion);	break;
			case SDL_MOUSEWHEEL:
				OnMouseScrolled(event.wheel);	break;
			case SDL_WINDOWEVENT:
				OnWindowResized(Application::Get().GetWindow());	break;
		}
	}

	void PerspectiveCamera::OnMouseMotion(SDL_MouseMotionEvent& e) {

		float mouseX = static_cast<float>(e.xrel);
		float mouseY = static_cast<float>(e.yrel);

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
	}

	void PerspectiveCamera::OnMouseScrolled(SDL_MouseWheelEvent& e)
	{
		float zoom = c_fov;
		zoom -= e.preciseY;
		zoom = std::max(zoom, 1.0f);
		zoom = std::min(zoom, 45.0f);
		c_PerspTraslationSpeed = zoom / 10;
		SetProjection(zoom, c_aspectRatio, c_nearPlane, c_farPlane);
	}

	void PerspectiveCamera::OnWindowResized(Window& e)
	{
		c_aspectRatio = { e.GetWidth(), e.GetHeight() };
		SetProjection(c_fov, c_aspectRatio, c_nearPlane, c_farPlane);
	}
}