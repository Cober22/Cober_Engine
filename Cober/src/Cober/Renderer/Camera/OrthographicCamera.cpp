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
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// --- Keyboard keys
		// Wihout Rotation
		if (keystate[SDL_SCANCODE_A])
			c_position.x -=c_OrthoTraslationSpeed * ts;
		if (keystate[SDL_SCANCODE_D])
			c_position.x += c_OrthoTraslationSpeed * ts;
		if (keystate[SDL_SCANCODE_S])
			c_position.y -= c_OrthoTraslationSpeed * ts;
		if (keystate[SDL_SCANCODE_W])
			c_position.y += c_OrthoTraslationSpeed * ts;
		
		// With Rotation
		/*if (keystate[SDL_SCANCODE_A]) {
			c_position.x -= cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y -= sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_D]) {
			c_position.x += cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y += sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_S]) {
			c_position.x -= -sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y -= cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_W]) {
			c_position.x += -sin(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
			c_position.y += cos(glm::radians(c_OrthoRotation)) * c_OrthoTraslationSpeed * ts;
		}*/
		
		if (keystate[SDL_SCANCODE_Q])
			c_OrthoRotation -= c_OrthoRotationSpeed * ts;
		if (keystate[SDL_SCANCODE_E]) 
			c_OrthoRotation += c_OrthoRotationSpeed * ts;
		
		c_OrthoTraslationSpeed = c_zoomLevel;
		c_OrthoRotationSpeed = c_zoomLevel * 10;
		
		// CONSTRAINS
		//c_position.y = std::max(c_position.y, 0.0f);
		//c_position.z = 0;
		
		RecalculateMatrix();
	}

	void OrthographicCamera::Resize(float width, float heigth) {

		float ratio = width / heigth;
		SetProjection(-ratio * c_zoomLevel, ratio * c_zoomLevel, -c_zoomLevel, c_zoomLevel);
	}
		
	void OrthographicCamera::OnEvent(SDL_Event& event)
	{
		switch (event.type) {
			case SDL_MOUSEMOTION:
				OnMouseMotion(event.motion);	break;
			case SDL_MOUSEWHEEL:
				OnMouseScrolled(event.wheel);	break;
			case SDL_WINDOWEVENT_RESIZED:
				OnWindowResized(Application::Get().GetWindow());	break;
		}
	}

	void OrthographicCamera::OnMouseMotion(SDL_MouseMotionEvent& e) {

		float mouseX = static_cast<float>(e.xrel);
		float mouseY = static_cast<float>(e.yrel);
		
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
	}

	void OrthographicCamera::OnMouseScrolled(SDL_MouseWheelEvent& e) {

		c_zoomLevel -= e.preciseY * 0.25f;
		c_zoomLevel = std::max(c_zoomLevel, 0.25f);
		c_zoomLevel = std::min(c_zoomLevel, 25.0f);
		
		float ratio = c_aspectRatio.x / c_aspectRatio.y;
		SetProjection(-ratio * c_zoomLevel, ratio * c_zoomLevel, -c_zoomLevel, c_zoomLevel);
	}

	void OrthographicCamera::OnWindowResized(Window& e) {
		
		Resize((float)e.GetWidth(), (float)e.GetHeight());
	}
}