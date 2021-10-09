#include "td_pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Thunder
{
	OrthographicCamera::OrthographicCamera(const glm::vec4& size, float nearPlane /* = -1.0f */, float farPlane /* = 1.0f */)
		: m_ViewMatrix(1.0f) 
	{
		SetProjectionMatrix(size, nearPlane, farPlane);
	}

	void OrthographicCamera::SetProjectionMatrix(const glm::vec4& size, float nearPlane /* = -1.0f */, float farPlane /* = 1.0f */)
	{
		TD_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(size[0], size[1], size[2], size[3], nearPlane, farPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		TD_PROFILE_FUNCTION();

		glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(cameraTransform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
