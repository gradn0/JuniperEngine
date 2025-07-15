#include "pch.h"
#include "Application.h"
#include "Input.h"

namespace Juniper {

	bool Input::IsKeyPressed(int key)
	{
		GLFWwindow* window = Application::Get().GetWindow().Get();
		int action = glfwGetKey(window, key);
		return (action == GLFW_PRESS || action == GLFW_REPEAT);
	}

	bool Input::IsKeyReleased(int key)
	{
		GLFWwindow* window = Application::Get().GetWindow().Get();
		int action = glfwGetKey(window, key);
		return (action == GLFW_RELEASE);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* window = Application::Get().GetWindow().Get();
		int action = glfwGetMouseButton(window, button);
		return (action == GLFW_PRESS);
	}

	bool Input::IsMouseButtonReleased(int button)
	{
		GLFWwindow* window = Application::Get().GetWindow().Get();
		int action = glfwGetMouseButton(window, button);
		return (action == GLFW_RELEASE);
	}

	std::pair<double, double> Input::GetMousePosition()
	{
		GLFWwindow* window = Application::Get().GetWindow().Get();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return std::pair<double, double>(x, y);
	}

}
