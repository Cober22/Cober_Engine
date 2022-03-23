#pragma once

#include <glm/glm.hpp>

namespace Cober {

	class PerspectiveCamera 
	{
	public:
		PerspectiveCamera(float angle, float aspectRatio, float nearPlane, float farPlane);

		void SetProjection(float angle, float aspectRatio, float nearPlane, float farPlane);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateProjectionMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateProjectionMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetModelViewProjectionMatrix() const { return m_ModelViewProjectionMatrix; }
	private:
		void RecalculateProjectionMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ModelMatrix;
		glm::mat4 m_ModelViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}