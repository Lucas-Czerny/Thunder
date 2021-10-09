#pragma once

#include <glm/glm.hpp>

namespace Thunder
{
	class Camera
	{
	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

	public:
		Camera() = default;
		Camera(const glm::mat4& projectionMatrix)
			: m_ProjectionMatrix(projectionMatrix) {}

		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
	};
}