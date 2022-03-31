#pragma once

#include "CameraController.h"
#include "Cober/Timestep.h"

#include <SDL/SDL.h>
#include <glm/glm.hpp>

namespace Cober {

	class OrthographicCamera : public CameraController
	{
	public:
		OrthographicCamera(glm::vec2 aspectRatio);
		OrthographicCamera(float left, float right, float bottom, float top);

		void OnUpdate(Timestep ts) override;
		void OnEvent(SDL_Event& event) override;

		const glm::mat4& GetViewMatrix() const override { return  c_VMatrix; }
		const glm::mat4& GetProjectionMatrix() const override { return  c_PMatrix; }
		const glm::mat4& GetModelMatrix() const override { return  c_MMatrix; };

		glm::vec3& GetPosition() { return c_position; }
		void SetPosition(glm::vec3& position) { c_position = position; }
		glm::vec3& GetDirection() { return c_direction; }
		void SetDirection(glm::vec3& direction) { c_direction = direction; }
	public:
		void OnMouseMotion(SDL_MouseMotionEvent& e) override;
		void OnMouseScrolled(SDL_MouseWheelEvent& e) override;
		void OnWindowResized(Window& e) override;
	};
}