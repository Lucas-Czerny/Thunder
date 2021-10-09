#include "td_pch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Thunder
{
	SceneCamera::SceneCamera(const ProjectionType& projectionType /* = ProjectionType::Orthographic */)
	{
		SetProjectionType(projectionType);
		CalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicParameters.Size = size;
		m_OrthographicParameters.Near = nearClip;
		m_OrthographicParameters.Far = farClip;

		CalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveParameters.VerticalFOV = verticalFOV;
		m_PerspectiveParameters.Near = nearClip;
		m_PerspectiveParameters.Far = farClip;

		CalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		CalculateProjection();
	}

	void SceneCamera::CalculateProjection()
	{
		switch (m_ProjectionType)
		{
			case ProjectionType::Orthographic:
			{
				float left = -m_AspectRatio * m_OrthographicParameters.Size / 2;
				float right = m_AspectRatio * m_OrthographicParameters.Size / 2;

				float bottom = -m_OrthographicParameters.Size / 2;
				float top = m_OrthographicParameters.Size / 2;
					
				m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicParameters.Near, m_OrthographicParameters.Far);

				break;
			}
			case ProjectionType::Perspective:
			{
				m_ProjectionMatrix = glm::perspective(m_PerspectiveParameters.VerticalFOV, m_AspectRatio, m_PerspectiveParameters.Near, m_PerspectiveParameters.Far);

				break;
			}
		}

	}
}
