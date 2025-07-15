#include <imgui/imgui.h>
#include "EditorLayer.h"
#include "Panels.h"

namespace Juniper {

	EditorLayer::EditorLayer() :
		Layer("Editor Layer"),
		m_App(Application::Get())
	{
	}

	bool EditorLayer::OnKeyPress(KeyPressEvent& event)
	{
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
	}

	void EditorLayer::OnUpdate(float dt)
	{
		updateCamera(dt);

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::Clear();
	}

	void EditorLayer::OnImGuiRender()
	{
		Panels::ScenePanel(m_App.GetActiveScene());
	}

	void EditorLayer::updateCamera(float dt)
	{
	}

}