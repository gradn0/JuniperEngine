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

	struct Capabilities
	{
		int TextureSlots = 32;
	};

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
	};

	class Renderer
	{
	public:
		static void Init();

		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetDepthMask(bool enabled);

		static void BeginScene(const OrthographicCamera& camera, const std::shared_ptr<Shader> shader);
		static void EndScene();

        // TODO: Think about using size + position over transform (faster, less consistent api)
		static void SubmitQuad(glm::mat4 transform, glm::vec4 color, const std::shared_ptr<Texture>& texture);

		static void OnWindowResize(int width, int height);

	private:
		static void resetBatch();
		static void flush();
		static void drawIndexed(const VertexArray& vertexArray, size_t indexCount, const Shader& shader);
		static void drawIndexed(const VertexArray& vertexArray, const Shader& shader);

		static void GLAPIENTRY messageCallback(
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

