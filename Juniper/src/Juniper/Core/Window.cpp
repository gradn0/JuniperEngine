#include "pch.h"
#include "Log.h"
#include "Window.h"

namespace Juniper {

	Window::Window(const WindowProps& props):
		m_WindowInfo(WindowInfo{props.Title, props.Width, props.Height, props.VSync })
	{
		// Initialise GLFW
		JP_CORE_VERIFY(glfwInit(), "Failed to initialise GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		#ifdef JP_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

		// Initialise window
		JP_CORE_INFO("Creating Window: \"{0}\" ({1}x{2})", m_WindowInfo.Title, m_WindowInfo.Width, m_WindowInfo.Height);
		m_Window = glfwCreateWindow(m_WindowInfo.Width, m_WindowInfo.Height, m_WindowInfo.Title.c_str(), nullptr, nullptr);
		JP_CORE_ASSERT(m_Window, "Failed to create window");

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowInfo);
		glfwSwapInterval(m_WindowInfo.VSync);
		
		// Input event callbacks
		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetCharCallback(m_Window, CharCallback);
		glfwSetCursorPosCallback(m_Window, CursorPositionCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetScrollCallback(m_Window, MouseScrollCallback);
		
		// Window event callbacks
		glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
		glfwSetWindowPosCallback(m_Window, WindowPositionCallback);
		glfwSetWindowFocusCallback(m_Window, WindowFocusCallback);
		glfwSetWindowCloseCallback(m_Window, WindowCloseCallback);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::SetEventCallBack(EventCallback cb)
	{
		m_WindowInfo.EventCallback = cb;
	}

	void Window::SetCursorDisabled(bool disabled)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		m_IsCursorDisabled = disabled;
	}

	void Window::ToggleCursorDisabled()
	{
		SetCursorDisabled(!m_IsCursorDisabled);
	}

	void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback)
		{
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				KeyPressEvent e(key, action == GLFW_REPEAT ? 1 : 0);
				info->EventCallback(e);
			}
			else if (action == GLFW_RELEASE)
			{
				KeyReleaseEvent e(key);
				info->EventCallback(e);
			}
		}
	}

	void Window::CharCallback(GLFWwindow* window, unsigned int c)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			CharEvent e(c);
			info->EventCallback(e);
		}
	}

	void Window::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			MouseMoveEvent e(xpos, ypos);
			info->EventCallback(e);
		}
	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback)
		{
			if (action == GLFW_PRESS)
			{
				MouseButtonPressEvent e(button);
				info->EventCallback(e);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleaseEvent e(button);
				info->EventCallback(e);
			}
		}
	}

	void Window::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			MouseScrollEvent e(xoffset, yoffset);
			info->EventCallback(e);
		}
	}

	void Window::WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			WindowResizeEvent e(width, height);
			info->EventCallback(e);
		}
	}

	void Window::WindowPositionCallback(GLFWwindow* window, int xpos, int ypos)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			WindowMoveEvent e(xpos, ypos);
			info->EventCallback(e);
		}
	}

	void Window::WindowFocusCallback(GLFWwindow* window, int focused)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			if (focused)
			{
				WindowFocusEvent e;
				info->EventCallback(e);
			}
			else {
				WindowFocusLostEvent e;
				info->EventCallback(e);
			}
		}
	}

	void Window::WindowCloseCallback(GLFWwindow* window)
	{
		WindowInfo* info = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		if (info->EventCallback) {
			WindowCloseEvent e;
			info->EventCallback(e);
		}
	}

}