#pragma once

#include "CameraController.h"
#include "Cober/Timestep.h"

#include <glm/glm.hpp>

namespace Cober {

	class OrthographicCamera : CameraController
	{
	public:
		OrthographicCamera(glm::vec2 aspectRatio);
		OrthographicCamera(float left, float right, float bottom, float top);

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event) override;
		void Resize(float width, float heigth) override;

		const glm::mat4& GetViewMatrix() const override { return  c_VMatrix; }
		const glm::mat4& GetProjectionMatrix() const override { return  c_PMatrix; }
		const glm::mat4& GetModelMatrix() const override { return  c_MMatrix; };

		glm::vec3& GetPosition() { return c_position; }
		void SetPosition(glm::vec3& position) { c_position = position; }
		glm::vec3& GetDirection() { return c_direction; }
		void SetDirection(glm::vec3& direction) { c_direction = direction; }
	private:
		bool OnMouseMotion(MouseMovedEvent& event) override;
		bool OnMouseScrolled(MouseScrolledEvent& event) override;
		bool OnWindowResized(WindowResizeEvent& event) override;
	private:
		float c_zoomLevel = 5.0f;
		float c_OrthoRotation = 0.0f;
		float c_OrthoTraslationSpeed = 1.0f, c_OrthoRotationSpeed = 10.0f;
	};
}