#pragma once

#include "Cober/Renderer/Camera/PerspectiveCamera.h"
#include "Cober/Timestep.h"

#include <SDL/SDL.h>

namespace Cober {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float angle, float aspectRatio, float nearPlane, float farPlane);

		void OnUpdate(Timestep ts);
		void OnEvent(SDL_Event& e);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(SDL_MouseWheelEvent& e);
		bool OnWindowResized(SDL_WindowEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		PerspectiveCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0, 0, 0 };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 20.0f;
	};

}