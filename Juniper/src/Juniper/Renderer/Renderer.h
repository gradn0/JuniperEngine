#pragma once

#include <optional>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Juniper/Core/Core.h"
#include "Juniper/Scene/Scene.h"
#include "Juniper/Renderer/Texture.h"
#include "Juniper/Renderer/Camera.h"
#include "Buffers.h"
#include "Shader.h"

namespace Juniper {

	class Renderer
	{
	public:
		static void Init();

		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetDepthMask(bool enabled);

		static void BeginScene(const OrthographicCamera& camera, const std::shared_ptr<Shader> shader);
		static void EndScene();
		static void Flush();

		static void SubmitQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
		static void SubmitQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, const std::shared_ptr<Texture2D>& texture);

		static void DrawIndexed(const VertexArray& vertexArray, size_t indexCount, const Shader& shader);
		static void DrawIndexed(const VertexArray& vertexArray, const Shader& shader);

		static void OnWindowResize(int width, int height);

	private:
		static void ResetBatch();

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

