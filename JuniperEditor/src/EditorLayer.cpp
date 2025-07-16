#include <imgui/imgui.h>
#include "glm/glm.hpp"
#include "EditorLayer.h"
#include "Panels.h"

namespace Juniper {

	EditorLayer::EditorLayer() :
		Layer("Editor Layer"),
		m_App(Application::Get()),
		m_Camera(1.6f / 0.9f, 1.0f)
	{
	}

	bool EditorLayer::OnKeyPress(KeyPressEvent& event)
	{
		// Toggle cursor
		if (event.Key == JP_KEY_LEFT_ALT)
		{
			m_App.GetWindow().ToggleCursorDisabled();
			m_HasCameraUpdated = false;
		}

		return true;
	}

	bool EditorLayer::OnMouseMove(MouseMoveEvent& event)
	{
		return true;
	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMoveEvent>(JP_BIND_HANDLER(EditorLayer::OnMouseMove));
		dispatcher.Dispatch<KeyPressEvent>(JP_BIND_HANDLER(EditorLayer::OnKeyPress));
	}

	void EditorLayer::OnAttach()
	{
		m_Shader = std::make_shared<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");
		m_Quad = Primitives::Quad(glm::vec4(1.0f));
		m_Texture = std::make_shared<Texture2D>("res/textures/leaf.jpg");
		m_Texture->Bind();
	}

	void EditorLayer::OnUpdate(float dt)
	{
		updateCamera(dt);

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::Clear();

		m_Shader->Bind();
		m_Shader->setUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
		m_Shader->setUniformMat4("u_Model", glm::mat4(1.0f));
		m_Shader->setUniform1i("u_Texture", 0);

		Renderer::DrawIndexed(*m_Quad, *m_Shader);
	}

	void EditorLayer::OnImGuiRender()
	{
		Panels::ScenePanel(m_App.GetActiveScene());
	}

	void EditorLayer::updateCamera(float dt)
	{
		if (!m_App.GetWindow().IsCursorDisabled()) return;

		float inc = m_CameraSpeed * m_Camera.GetZoom() * dt;

		if (Input::IsKeyPressed(JP_KEY_W))
			m_Camera.Translate(glm::vec3(0.0f, 1.0f, 0.0f) * inc);
		if (Input::IsKeyPressed(JP_KEY_A))
			m_Camera.Translate(glm::vec3(1.0f, 0.0f, 0.0f) * -inc);
		if (Input::IsKeyPressed(JP_KEY_S))
			m_Camera.Translate(glm::vec3(0.0f, 1.0f, 0.0f) * -inc);
		if (Input::IsKeyPressed(JP_KEY_D))
			m_Camera.Translate(glm::vec3(1.0f, 0.0f, 0.0f) * inc);
		if (Input::IsKeyPressed(JP_KEY_E))
			m_Camera.SetZoom(m_Camera.GetZoom() - inc);
		if (Input::IsKeyPressed(JP_KEY_Q))
			m_Camera.SetZoom(m_Camera.GetZoom() + inc);
	}
}