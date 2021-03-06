#pragma once

#include "Cober/Renderer/Camera/Camera.h"

#include <glm/glm.hpp>

namespace Cober {

	class SceneCamera : public Camera
	{
	public:	
		enum class CameraType {
			Perspective = 0,
			Orthographic = 1,
			FirstPerson = 2,
			TopDown = 3,
			RPG = 4,
			ActionRPG = 5
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);
	public:
		// Perspective
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void  SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void  SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void  SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
	public:
		// Orthographic
		float GetOrthographicSize() const { return m_OrthographicSize; }
		void  SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void  SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void  SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		CameraType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(CameraType type) { m_ProjectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		// Perspective
		CameraType m_ProjectionType = CameraType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
	private:
		// Orthographic
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};

}