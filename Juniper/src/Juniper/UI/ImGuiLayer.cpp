#include "pch.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "ImGuiRenderer.h"
#include "Juniper/Core/Core.h"
#include "Juniper/Core/Log.h"
#include "Juniper/Core/Application.h"
#include "Juniper/Events/Event.h"
#include "ImGuiLayer.h"

#define IS_PRESSED(x) ImGui::IsKeyPressed(toImGuiKey(JP_KEY_LEFT_CONTROL))

namespace Juniper {

	static ImGuiKey toImGuiKey(int keycode)
	{
		switch (keycode)
		{
			case JP_KEY_TAB: return ImGuiKey_Tab;
			case JP_KEY_LEFT: return ImGuiKey_LeftArrow;
			case JP_KEY_RIGHT: return ImGuiKey_RightArrow;
			case JP_KEY_UP: return ImGuiKey_UpArrow;
			case JP_KEY_DOWN: return ImGuiKey_DownArrow;
			case JP_KEY_PAGE_UP: return ImGuiKey_PageUp;
			case JP_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
			case JP_KEY_HOME: return ImGuiKey_Home;
			case JP_KEY_END: return ImGuiKey_End;
			case JP_KEY_INSERT: return ImGuiKey_Insert;
			case JP_KEY_DELETE: return ImGuiKey_Delete;
			case JP_KEY_BACKSPACE: return ImGuiKey_Backspace;
			case JP_KEY_SPACE: return ImGuiKey_Space;
			case JP_KEY_ENTER: return ImGuiKey_Enter;
			case JP_KEY_ESCAPE: return ImGuiKey_Escape;

			case JP_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
			case JP_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
			case JP_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
			case JP_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
			case JP_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
			case JP_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
			case JP_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
			case JP_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;

			case JP_KEY_A: return ImGuiKey_A;
			case JP_KEY_C: return ImGuiKey_C;
			case JP_KEY_V: return ImGuiKey_V;
			case JP_KEY_X: return ImGuiKey_X;
			case JP_KEY_Y: return ImGuiKey_Y;
			case JP_KEY_Z: return ImGuiKey_Z;
			case JP_KEY_S: return ImGuiKey_S;
			case JP_KEY_F: return ImGuiKey_F;
		}

		return ImGuiKey_0;
	}

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer"), m_Time(0.0f)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnEvent(Juniper::Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<KeyPressEvent>(JP_BIND_HANDLER(ImGuiLayer::OnKeyPressEvent));
		dispatcher.Dispatch<KeyReleaseEvent>(JP_BIND_HANDLER(ImGuiLayer::OnKeyReleaseEvent));
		dispatcher.Dispatch<CharEvent>(JP_BIND_HANDLER(ImGuiLayer::OnCharEvent));
		dispatcher.Dispatch<MouseMoveEvent>(JP_BIND_HANDLER(ImGuiLayer::OnMouseMoveEvent));
		dispatcher.Dispatch<MouseButtonPressEvent>(JP_BIND_HANDLER(ImGuiLayer::OnMouseButtonPressEvent));
		dispatcher.Dispatch<MouseButtonReleaseEvent>(JP_BIND_HANDLER(ImGuiLayer::OnMouseButtonReleaseEvent));
		dispatcher.Dispatch<MouseScrollEvent>(JP_BIND_HANDLER(ImGuiLayer::OnMouseScrollEvent));
		dispatcher.Dispatch<WindowResizeEvent>(JP_BIND_HANDLER(ImGuiLayer::OnWindowResizeEvent));
		dispatcher.Dispatch<WindowFocusEvent>(JP_BIND_HANDLER(ImGuiLayer::OnWindowFocusEvent));
		dispatcher.Dispatch<WindowFocusLostEvent>(JP_BIND_HANDLER(ImGuiLayer::OnWindowFocusLostEvent));
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnUpdate(float dt)
	{
		// Delta time
		float time = (float)glfwGetTime(); // TODO: Don't use glfw
		ImGui::GetIO().DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f) / (60.0f);
		m_Time = time;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(
			(float)app.GetWindow().GetWidth(),
			(float)app.GetWindow().GetHeight()
		);

 		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImGuiLayer::OnKeyPressEvent(KeyPressEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(toImGuiKey(e.Key), true);

		io.KeyCtrl = IS_PRESSED(JP_KEY_LEFT_CONTROL) || IS_PRESSED(JP_KEY_LEFT_CONTROL);
		io.KeyShift = IS_PRESSED(JP_KEY_LEFT_SHIFT) || IS_PRESSED(JP_KEY_RIGHT_SHIFT);
		io.KeyAlt = IS_PRESSED(JP_KEY_LEFT_ALT) || IS_PRESSED(JP_KEY_LEFT_ALT);
		io.KeySuper = IS_PRESSED(JP_KEY_LEFT_SUPER) || IS_PRESSED(JP_KEY_LEFT_SUPER);

		return false;
	}

	bool ImGuiLayer::OnKeyReleaseEvent(KeyReleaseEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(toImGuiKey(e.Key), false);
		return false;
	}

	bool ImGuiLayer::OnCharEvent(CharEvent& e)
	{
		ImGui::GetIO().AddInputCharacter(e.Char);
		return false;
	}

	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& e)
	{
		ImGui::GetIO().MousePos = ImVec2((float)e.X, (float)e.Y);
		return false;
	}

	bool ImGuiLayer::OnMouseButtonPressEvent(MouseButtonPressEvent& e)
	{
		ImGui::GetIO().MouseDown[e.Button] = true;
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& e)
	{
		ImGui::GetIO().MouseDown[e.Button] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)e.XOffset;
		io.MouseWheel += (float)e.YOffset;
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGui::GetIO().DisplaySize = ImVec2((float)e.Width, (float)e.Height);
		return false;
	}

	bool ImGuiLayer::OnWindowFocusEvent(WindowFocusEvent& e)
	{
		ImGui::GetIO().AddFocusEvent(true);
		return false;
	}

	bool ImGuiLayer::OnWindowFocusLostEvent(WindowFocusLostEvent& e)
	{
		ImGui::GetIO().AddFocusEvent(false);
		return false;
	}

}
