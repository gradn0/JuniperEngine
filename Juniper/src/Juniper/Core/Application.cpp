#include "pch.h"
#include "GLFW/glfw3.h"

#include "Juniper/UI/ImGuiLayer.h"
#include "Juniper/Renderer/Buffers.h"
#include "Juniper/Renderer/Renderer.h"
#include "Log.h"
#include "Application.h"

#define BIND_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Juniper {

	Application* Application::s_Instance = nullptr;

	Application::Application(const WindowProps& props)
	{
		s_Instance = this;
		m_Window = std::make_unique<Window>(props);
		m_Window->SetEventCallBack(BIND_FN(OnEvent));
		m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		Renderer::Init();
	}

	Application::~Application() {}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			// Delta time calculations
			float time = (float) glfwGetTime(); // TODO: de-couple from glfw
			float dt = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Update from the bottom of the layer stack to the top
			for (auto& layer : m_LayerStack)
				layer->OnUpdate(dt);

            // Render ImGui layer
			m_ImGuiLayer->Begin();
			for (auto& layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		// Handle close event
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {
			m_IsRunning = false;
			return true;
		});

		// Propagate events from the top of the layer stack down until handled
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(--it)->get()->OnEvent(e);
			if (e.IsHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.Push(layer);
		layer->OnAttach();
	}

	void Application::PopLayer()
	{
		m_LayerStack.Pop();
	}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	Scene& Application::GetActiveScene()
	{
		return m_ActiveScene;
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

}