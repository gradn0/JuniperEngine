#pragma once

#include "glm/glm.hpp"

namespace Juniper {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float aspectRatio, float zoom);
		~OrthographicCamera() = default;

		void SetPosition(const glm::vec3& position);
		void SetRotation(float degrees);
		void SetZoom(float zoom);

		inline glm::vec3 GetPosition() const { return m_Position; }
		inline float GetRotation() const { return m_Rotation; }
		inline float GetZoom() const { return m_Zoom; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void Translate(const glm::vec3& translation);

		void CalculateViewMatrix();
		void CalculateProjectionMatrix();

		bool IsVisible(const glm::vec3& worldPosition) const
		{
			glm::vec4 clipSpacePosition = m_ViewProjectionMatrix * glm::vec4(worldPosition, 1.0f);
			glm::vec3 ndcPosition = glm::vec3(clipSpacePosition) / clipSpacePosition.w;

			return ndcPosition.x >= -1.0f && ndcPosition.x <= 1.0f &&
				ndcPosition.y >= -1.0f && ndcPosition.y <= 1.0f &&
				ndcPosition.z >= -1.0f && ndcPosition.z <= 1.0f;
		}

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix{ 0.0f };
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position{ 0.0f };
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;
		float m_AspectRatio = 1.6f / 0.9f;

		bool m_HasChanged = false;
	};

}

