#pragma once

#include "glm/glm.hpp"

namespace Juniper {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() = default;

		void SetPosition(const glm::vec3& position);
		void SetRotation(float degrees);
		void SetZoom(float zoom);
		void Translate(const glm::vec3& translation);

		void CalculateViewMatrix();
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec3 GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix{ 0.0f };
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position{ 0.0f };
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;

		bool m_HasChanged = false;
	};

}

