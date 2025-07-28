#include <imgui/imgui.h>
#include "glm/glm.hpp"
#include "EditorLayer.h"
#include "Panels.h"
#include "Juniper/Scene/Tilemap.h"

namespace Juniper {

	EditorLayer::EditorLayer() :
		Layer("Editor Layer"),
		m_App(Application::Get()),
		m_Camera(std::make_shared<OrthographicCamera>(1.6f / 0.9f, 10.0f))
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
		Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

		m_Shader = std::make_shared<Shader>("res/shaders/vertex.shader", "res/shaders/fragment.shader");

		auto& scene = m_App.GetActiveScene();
		auto tilemap = scene.CreateEntity();
		scene.AddComponent<TagComponent>(tilemap, "Tilemap");
		scene.AddComponent<TransformComponent>(tilemap);
		scene.AddComponent<TilemapComponent>(tilemap, std::make_shared<Tilemap>("res/tiling/testmap.tmx"));

		auto sprite = scene.CreateEntity();
		scene.AddComponent<TagComponent>(sprite, "Sprite");
		scene.AddComponent<TransformComponent>(sprite);
		scene.AddComponent<SpriteComponent>(sprite);
	}

	void EditorLayer::OnUpdate(float dt)
	{
		updateCamera(dt);
		
		Renderer::Clear();
		Renderer::BeginScene(m_Camera, m_Shader);

		m_App.GetActiveScene().OnUpdate(dt);

		Renderer::EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{
		Panels::ScenePanel(m_App.GetActiveScene());
		Panels::StatsPanel();
	}

	void EditorLayer::updateCamera(float dt)
	{
		if (!m_App.GetWindow().IsCursorDisabled()) return;

		float inc = m_CameraSpeed * m_Camera->GetZoom() * dt;

		if (Input::IsKeyPressed(JP_KEY_W))
			m_Camera->Translate(glm::vec3(0.0f, 1.0f, 0.0f) * inc);
		if (Input::IsKeyPressed(JP_KEY_A))
			m_Camera->Translate(glm::vec3(1.0f, 0.0f, 0.0f) * -inc);
		if (Input::IsKeyPressed(JP_KEY_S))
			m_Camera->Translate(glm::vec3(0.0f, 1.0f, 0.0f) * -inc);
		if (Input::IsKeyPressed(JP_KEY_D))
			m_Camera->Translate(glm::vec3(1.0f, 0.0f, 0.0f) * inc);
		if (Input::IsKeyPressed(JP_KEY_E))
			m_Camera->SetZoom(m_Camera->GetZoom() - inc);
		if (Input::IsKeyPressed(JP_KEY_Q))
			m_Camera->SetZoom(m_Camera->GetZoom() + inc);
	}
}