#include "pch.h"
#include "OrthographicCameraController.h"

namespace Cober {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// --- Keyboard keys
		if (keystate[SDL_SCANCODE_A]) { m_CameraPosition.x -= m_CameraTranslationSpeed * ts; }
		if (keystate[SDL_SCANCODE_D]) { m_CameraPosition.x += m_CameraTranslationSpeed * ts; }
		if (keystate[SDL_SCANCODE_S]) { m_CameraPosition.y -= m_CameraTranslationSpeed * ts; }
		if (keystate[SDL_SCANCODE_W]) { m_CameraPosition.y += m_CameraTranslationSpeed * ts; }

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(SDL_Event& e)
	{
		if (e.type == SDL_MOUSEWHEEL)
			OnMouseScrolled(e.wheel);
		if (e.type == SDL_WINDOWEVENT_SIZE_CHANGED)
			OnWindowResized(e.window);
	}

	bool OrthographicCameraController::OnMouseScrolled(SDL_MouseWheelEvent& e)
	{
		m_ZoomLevel -= e.preciseY * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_ZoomLevel = std::min(m_ZoomLevel, 8.0f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(SDL_WindowEvent& e)
	{
		m_AspectRatio = (float)e.data1 / (float)e.data2;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}