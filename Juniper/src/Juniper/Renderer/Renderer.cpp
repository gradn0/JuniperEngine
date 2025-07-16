#include "pch.h"
#include "GLFW/glfw3.h"
#include "Juniper/Core/Log.h"
#include "Juniper/Scene/Scene.h"
#include "Renderer.h"

namespace Juniper {

	void Renderer::Init()
	{
		// Initialise opengl
		JP_CORE_VERIFY(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load OpenGL");
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetDepthMask(bool enabled)
	{
		glDepthMask(enabled);
	}

	void Renderer::DrawIndexed(const VertexArray& vertexArray, const Shader& shader)
	{
		vertexArray.Bind();
		shader.Bind();
		glDrawElements(GL_TRIANGLES, vertexArray.GetIndexCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::OnWindowResize(size_t width, size_t height)
	{
		glViewport(0, 0, width, height);
	}

	void GLAPIENTRY Renderer::MessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		JP_CORE_ERROR("{0} type = 0x{1}, severity = 0x{2}, message = {3}",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message
		);
		__debugbreak();
	}

}
