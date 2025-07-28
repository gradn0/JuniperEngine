#include "pch.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Juniper/Core/Log.h"

Juniper::OrthographicCamera::OrthographicCamera(float aspectRatio, float zoom) :
	m_AspectRatio(aspectRatio),
	m_Zoom(zoom)
{
	CalculateProjectionMatrix();
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
	if (zoom < 0.1f)
		zoom = 0.1f;

	m_Zoom = zoom;
	CalculateProjectionMatrix();
	CalculateViewMatrix();
}

void Juniper::OrthographicCamera::CalculateViewMatrix()
{
	glm::mat4 transform = 
		glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Juniper::OrthographicCamera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, -1.0f, 1.0f);
}
