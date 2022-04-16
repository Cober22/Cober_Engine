#pragma once

#include "CameraController.h"
#include "Cober/Timestep.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	class PerspectiveCamera : public CameraController
	{
	public:
		PerspectiveCamera(float fov, glm::vec2 aspectRatio, float nearPlane, float farPlane);
		void OnUpdate(Timestep ts) override;
		void OnEvent(SDL_Event& event) override;

		const glm::mat4& GetViewMatrix() const override { return  glm::lookAt(c_position, c_position + c_direction, c_upAxis); }
		const glm::mat4& GetProjectionMatrix() const override { return  c_PMatrix; }
		const glm::mat4& GetModelMatrix() const override { return c_MMatrix; }

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