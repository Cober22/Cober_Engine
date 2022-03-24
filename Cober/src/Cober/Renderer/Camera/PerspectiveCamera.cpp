#include "pch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp>

namespace Cober {

	PerspectiveCamera::PerspectiveCamera(float angle, float aspectRatio, float nearPlane, float farPlane)
		: m_ProjectionMatrix(glm::perspective(angle, aspectRatio, nearPlane, farPlane)), m_ViewMatrix(1.0f), m_ModelMatrix(1.0f)
	{
		m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		// TEST
	 	//m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));


		m_ModelViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;
	}

	void PerspectiveCamera::SetProjection(float angle, float aspectRatio, float nearPlane, float farPlane)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(angle), aspectRatio, nearPlane, farPlane);

		m_ModelViewProjectionMatrix = m_ModelMatrix * m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateMVPMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ModelViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;
	}
}