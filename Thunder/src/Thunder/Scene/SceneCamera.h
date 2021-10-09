#pragma once

#include "Thunder/Renderer/Camera.h"

namespace Thunder
{
	enum class ProjectionType
	{
		Orthographic = 0, Perspective = 1
	};

	class SceneCamera : public Camera
	{
	private:
		struct OrthographicParameters
		{
			float Size = 1.0f;
			float Near = 1.0f, Far = -1.0f;
		} m_OrthographicParameters;

		struct PerspectiveParameters
		{
			float VerticalFOV = glm::radians(45.0f);
			float Near = 0.001f, Far = 10000.0f;
		} m_PerspectiveParameters;

		float m_AspectRatio = 1.0f;

		ProjectionType m_ProjectionType = ProjectionType::Perspective;

	public:
		SceneCamera(const ProjectionType& projectionType = ProjectionType::Perspective);

		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(const ProjectionType& projectionType) { m_ProjectionType = projectionType; CalculateProjection(); }

		// ORTHOGRAPHIC

		void SetOrthographic(float size, float nearClip, float farClip);

		float GetOrthographicSize() const { return m_OrthographicParameters.Size; }
		void SetOrthographicSize(float size) { m_OrthographicParameters.Size = size; CalculateProjection(); }

		void SetOrthographicNear(float orthographicNear) { m_OrthographicParameters.Near = orthographicNear; CalculateProjection(); }
		float GetOrthographicNear() const { return m_OrthographicParameters.Near; }

		void SetOrthographicFar(float orthographicFar) { m_OrthographicParameters.Far = orthographicFar; CalculateProjection(); }
		float GetOrthographicFar() const { return m_OrthographicParameters.Far; }
		
		// PERSPECTIVE

		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		float GetPerspectiveVerticalFOV() const { return m_PerspectiveParameters.VerticalFOV; }
		void SetPerspectiveVerticalFOV(float size) { m_PerspectiveParameters.VerticalFOV = size; CalculateProjection(); }

		void SetPerspectiveNear(float PerspectiveNear) { m_PerspectiveParameters.Near = PerspectiveNear; CalculateProjection(); }
		float GetPerspectiveNear() const { return m_PerspectiveParameters.Near; }

		void SetPerspectiveFar(float PerspectiveFar) { m_PerspectiveParameters.Far = PerspectiveFar; CalculateProjection(); }
		float GetPerspectiveFar() const { return m_PerspectiveParameters.Far; }

	private:
		void CalculateProjection();
	};
}