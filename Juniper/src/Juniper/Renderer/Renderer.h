#pragma once

#include <optional>
#include "glad/glad.h"
#include "Juniper/Core/Core.h"
#include "Juniper/Scene/Scene.h"
#include "Juniper/Scene/Components.h"
#include "Buffers.h"
#include "Shader.h"

namespace Juniper {

	class Renderer
	{
	public:
		static Renderer& Get() { return *s_Instance; }

		static void Init();
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetDepthMask(bool enabled);

		static void DrawIndexed(const VertexArray& vertexArray, const Shader& shader);

	private:
		static Renderer* s_Instance;

		static void GLAPIENTRY MessageCallback(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam
		);
	};

}

