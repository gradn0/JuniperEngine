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
		m_Quad = Primitives::Quad(glm::vec4(1.0f));
		m_TextureLeaf = std::make_shared<Texture2D>("res/textures/leaf.jpg");
		m_TextureStone = std::make_shared<Texture2D>("res/textures/stone.jpg");
		m_SpritesheetFloor = std::make_shared<Texture2D>("res/textures/floors.png");

		m_Grass = std::make_shared<SubTexture2D>(m_SpritesheetFloor, glm::vec2(0.0f, 4.0f), glm::vec2(5.0f, 5.0f), 16.0f);

		auto& scene = m_App.GetActiveScene();

		auto quad = scene.CreateEntity();
		scene.AddComponent<TransformComponent>(quad, TransformComponent{
			glm::vec3{10.0f, 0.0f, 0.0f},
			glm::vec3{0.0f, 0.0f, 45.0f},
			glm::vec3{ 10.0f }
		});
		scene.AddComponent<SpriteComponent>(quad, SpriteComponent{ { 16.0f, 16.0 }, glm::vec4{ 1.0f }, m_Grass });
	}

	void EditorLayer::OnUpdate(float dt)
	{
		updateCamera(dt);

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::Clear();

		Renderer::BeginScene(m_Camera, m_Shader);

		m_App.GetActiveScene().OnUpdate(dt);

		TransformComponent t{};
		t.Scale = { 5.0f, 5.0f, 5.0f };

		for (size_t y = 0; y < 25; y++)
			for (size_t x = 0; x < 25; x++)
			{
				t.Translation = glm::vec4(x * 5.0f, y * 5.0f, 0.0f, 0.0f);
				auto& texture = (x + y) % 2 == 0 ? m_TextureLeaf : m_TextureStone;
				Renderer::SubmitQuad(t.GetTransform(), glm::vec4{ 1.0f }, texture);
			}

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