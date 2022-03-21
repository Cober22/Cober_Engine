#pragma once

#include "Cober/Renderer/Camera/OrthographicCamera.h"
#include "Cober/Timestep.h"

#include <SDL/SDL.h>

namespace Cober {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(SDL_Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(SDL_MouseWheelEvent& e);
		bool OnWindowResized(SDL_WindowEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0, 0, 0 };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 1.0f;
	};

}