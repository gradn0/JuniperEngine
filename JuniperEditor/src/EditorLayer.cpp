#include <imgui/imgui.h>
#include "glm/glm.hpp"
#include "EditorLayer.h"
#include "Panels.h"

namespace Juniper {

	EditorLayer::EditorLayer() :
		Layer("Editor Layer"),
		m_App(Application::Get()),
		m_Camera(1.6f / 0.9f, 10.0f)
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
		m_Tilemap = std::make_shared<Tilemap>("res/tiling/testmap.tmx");
	}

	void EditorLayer::OnUpdate(float dt)
	{
		updateCamera(dt);

		Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Renderer::Clear();

		Renderer::BeginScene(m_Camera, m_Shader);
		Renderer::SubmitTilemap(m_Tilemap);
		Renderer::EndScene();
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