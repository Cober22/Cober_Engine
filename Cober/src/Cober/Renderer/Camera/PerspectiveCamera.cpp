#include "pch.h"
#include "PerspectiveCamera.h"

//#include <glm/gtc/matrix_transform.hpp>  

namespace Cober {

	PerspectiveCamera::PerspectiveCamera(float fov, glm::vec2 aspectRatio, float nearPlane, float farPlane)
		: c_PMatrix(glm::perspective(glm::radians(fov), aspectRatio.x / aspectRatio.y, nearPlane, farPlane)), 
		  c_VMatrix(1.0f),
		  c_MMatrix(1.0f),
		  c_MVPMatrix(c_PMatrix* c_VMatrix* c_MMatrix)
	{
		c_fov = fov;
		c_aspectRatio = aspectRatio;
		c_nearPlane = nearPlane;
		c_farPlane = farPlane;
	}

	void PerspectiveCamera::SetProjection(float fov, glm::vec2 aspectRatio, float nearPlane, float farPlane)
	{
		c_PMatrix = glm::perspective(glm::radians(fov), aspectRatio.x / aspectRatio.y, nearPlane, farPlane);
		c_fov = fov;
		c_aspectRatio = aspectRatio;
		c_nearPlane = nearPlane;
		c_farPlane = farPlane;
		c_MVPMatrix = c_PMatrix * c_VMatrix * c_MMatrix;
	}

	void PerspectiveCamera::RecalculateMVPMatrix(){

		c_rightAxis = glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, c_direction));
		c_upAxis = glm::cross(c_direction, c_rightAxis);

		LookAt(c_position, c_position + c_direction, c_upAxis);
		c_MVPMatrix = c_PMatrix * c_VMatrix * c_MMatrix;
	}

	void PerspectiveCamera::LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 upAxis) {
		c_position = position;
		c_upAxis = upAxis;

		c_VMatrix = glm::lookAt(c_position, target, c_upAxis);
	}

	void PerspectiveCamera::OnUpdate(Timestep ts) {

		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		
		// --- Keyboard keys
		if (keystate[SDL_SCANCODE_A])
			c_position -= glm::normalize(glm::cross(c_direction, c_upAxis)) * c_traslationSpeed * glm::vec3(ts);
		if (keystate[SDL_SCANCODE_D])
			c_position += glm::normalize(glm::cross(c_direction, c_upAxis)) * c_traslationSpeed * glm::vec3(ts);
		if (keystate[SDL_SCANCODE_S])
			c_position -= c_direction * c_traslationSpeed * glm::vec3(ts);
		if (keystate[SDL_SCANCODE_W])										
			c_position += c_direction * c_traslationSpeed * glm::vec3(ts);

		if (keystate[SDL_SCANCODE_Q]) 
			c_yaw -= c_yawSpeed * c_CamSensitivity * ts;
		if (keystate[SDL_SCANCODE_E])
			c_yaw += c_yawSpeed * c_CamSensitivity * ts;
		
		glm::vec3 front;
		front.x = cos(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		front.y = sin(glm::radians(c_pitch));
		front.z = sin(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		c_direction = glm::normalize(front);

		// To stay on the ground
		c_position.y = 0.0f;

		RecalculateMVPMatrix();
	}

	void PerspectiveCamera::OnEvent(SDL_Event& event) {
		
		switch (event.type) {
			case SDL_MOUSEMOTION:
				OnMouseMotion(event.motion);	break;
			case SDL_MOUSEWHEEL:
				OnMouseScrolled(event.wheel);	break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				OnWindowResized(event.window);	break;
		}
	}

	void PerspectiveCamera::OnMouseMotion(SDL_MouseMotionEvent& e) {

		float mouseX = static_cast<float>(e.x);
		float mouseY = static_cast<float>(e.y);

		if(m_firstMouse)
		{
			lastX = mouseX;
			lastY = mouseY;
			m_firstMouse = false;
		}

		float xoffset = mouseX - lastX;
		float yoffset = lastY - mouseY;	// Reversed because y-coords go from bottom to top
		lastX = mouseX;
		lastY = mouseY;

		xoffset *= m_mouseSensitivity;
		yoffset *= m_mouseSensitivity;

		c_yaw += xoffset;
		c_pitch += yoffset;

		c_pitch = std::max(c_pitch, -89.0f);
		c_pitch = std::min(c_pitch, 89.0f);

		glm::vec3 front;
		front.x = cos(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		front.y = sin(glm::radians(c_pitch));
		front.z = sin(glm::radians(c_yaw)) * cos(glm::radians(c_pitch));
		c_direction = glm::normalize(front);

		RecalculateMVPMatrix();
	}

	void PerspectiveCamera::OnMouseScrolled(SDL_MouseWheelEvent& e)
	{
		float zoom = c_fov;
		zoom -= e.preciseY;
		zoom = std::max(zoom, 1.0f);
		zoom = std::min(zoom, 45.0f);
		c_traslationSpeed = zoom / 10;
		SetProjection(zoom, c_aspectRatio, c_nearPlane, c_farPlane);
	}

	void PerspectiveCamera::OnWindowResized(SDL_WindowEvent& e)
	{
		c_aspectRatio = { (float)e.data1, (float)e.data2 };
		SetProjection(c_fov, c_aspectRatio, c_nearPlane, c_farPlane);
	}
}