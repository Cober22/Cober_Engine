#include "pch.h"
#include "PerspectiveCameraController.h"

namespace Cober {

	PerspectiveCameraController::PerspectiveCameraController(float angle, float aspectRatio, float nearPlane, float farPlane)
		: m_AspectRatio(aspectRatio), m_Camera(angle, aspectRatio, nearPlane, farPlane), m_Rotation(0.0f)
	{
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// --- Keyboard keys
		if (keystate[SDL_SCANCODE_A]) {
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_D]) {
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_S]) {
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_W]) {
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_Q]) {
			m_CameraRotation += -sin(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
			m_CameraRotation += cos(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
		}
		if (keystate[SDL_SCANCODE_E]) {
			m_CameraRotation -= -sin(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
			m_CameraRotation -= cos(glm::radians(m_CameraRotation)) * m_CameraRotationSpeed * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void PerspectiveCameraController::OnEvent(SDL_Event& e)
	{
		if (e.type == SDL_MOUSEWHEEL)
			OnMouseScrolled(e.wheel);
		if (e.type == SDL_WINDOWEVENT_SIZE_CHANGED)
			OnWindowResized(e.window);
	}

	bool PerspectiveCameraController::OnMouseScrolled(SDL_MouseWheelEvent& e)
	{
		/*m_ZoomLevel -= e.preciseY * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_ZoomLevel = std::min(m_ZoomLevel, 8.0f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);*/

		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(SDL_WindowEvent& e)
	{
		/*m_AspectRatio = (float)e.data1 / (float)e.data2;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);*/

		return false;
	}
}