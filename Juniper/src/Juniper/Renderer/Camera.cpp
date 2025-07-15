#include "pch.h"
#include "Camera.h"
#include "Juniper/Core/Log.h"
#include "glm/gtc/matrix_transform.hpp"

Juniper::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
	m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
{
	CalculateViewMatrix();
}

void Juniper::OrthographicCamera::SetPosition(const glm::vec3& position)
{
	m_Position = position;
	CalculateViewMatrix();
}

void Juniper::OrthographicCamera::Translate(const glm::vec3& translation)
{
	m_Position += translation;
	CalculateViewMatrix();
}

void Juniper::OrthographicCamera::SetRotation(float degrees)
{
	m_Rotation = degrees;
	CalculateViewMatrix();
}

void Juniper::OrthographicCamera::SetZoom(float zoom)
{
}

void Juniper::OrthographicCamera::CalculateViewMatrix()
{
	glm::mat4 transform = 
		glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
