#pragma once

#include "pch.h"
#include "Juniper/Events/Event.h"
#include "Juniper/Scene/Scene.h"
#include "Core.h"
#include "Window.h"
#include "Layer.h"

namespace Juniper {

	class ImGuiLayer;

	class Application
	{
	public:
		explicit Application(const WindowProps& props = WindowProps());
		virtual ~Application();

		void Run();
		virtual void OnEvent(Event& e);
		virtual void PushLayer(Layer* layer);
		virtual void PopLayer();

		Window& GetWindow();
		Scene& GetActiveScene();

		static Application& Get();

	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		Scene m_ActiveScene;
		LayerStack m_LayerStack;

		bool m_IsRunning = true;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	};

	// Defined in client
	Application* CreateApplication();

}