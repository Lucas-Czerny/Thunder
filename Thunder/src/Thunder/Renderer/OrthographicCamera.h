#pragma once

#include <glm/glm.hpp>

namespace Thunder
{
	class OrthographicCamera
	{
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		
	public:
		OrthographicCamera(const glm::vec4& size, float nearPlane = -1.0f, float farPlane = 1.0f);

		void SetProjectionMatrix(const glm::vec4& size, float nearPlane = -1.0f, float farPlane = 1.0f);

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
	};
}