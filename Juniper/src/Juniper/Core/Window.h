#pragma once

#include "pch.h"
#include <GLFW/glfw3.h>
#include "Juniper/Events/Event.h"
#include "Juniper/Events/InputEvents.h"
#include "Juniper/Events/WindowEvents.h"
#include "Core.h"

namespace Juniper {

	using EventCallback = std::function<void(Event&)>;

	struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync;

		WindowProps(
			const std::string& title = "Untitled",
			uint32_t width = 1280,
			uint32_t height = 720,
			bool vsync = true
		) : Title(title), Width(width), Height(height), VSync(vsync) {}
	};

	struct WindowInfo {
		std::string Title;
		uint32_t Width, Height;
		bool VSync;
		EventCallback EventCallback;
	};

	class Window
	{
	public:
		Window(const WindowProps& props);
		~Window();

		void OnUpdate();
		void SetEventCallBack(EventCallback cb);
		void SetCursorDisabled(bool disabled);
		void ToggleCursorDisabled();

		inline GLFWwindow* Get() { return m_Window; }
		inline uint32_t GetWidth() const { return m_WindowInfo.Width; }
		inline uint32_t GetHeight() const { return m_WindowInfo.Height; }
		inline bool IsCursorDisabled() const { return m_IsCursorDisabled; }

	private:
		GLFWwindow* m_Window;
		WindowInfo m_WindowInfo;
		bool m_IsCursorDisabled = false;

		// Input callbacks
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, unsigned int c);
		static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		// Window callbacks
		static void WindowResizeCallback(GLFWwindow* window, int width, int height);
		static void WindowPositionCallback(GLFWwindow* window, int xpos, int ypos);
		static void WindowFocusCallback(GLFWwindow* window, int focused);
		static void WindowCloseCallback(GLFWwindow* window);
	};

}
