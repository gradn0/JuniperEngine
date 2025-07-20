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

	// Supplied to the internal submit method by the public submission api
	struct QuadSpec
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 Size;
		std::array<glm::vec2, 4> TexCoords;
		std::shared_ptr<Texture2D> Texture;
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

		static void SubmitQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
		static void SubmitQuad(glm::vec3 position, glm::vec2 size, const std::shared_ptr<Texture2D>& texture);
		static void SubmitQuad(glm::vec3 position, glm::vec2 size, const std::shared_ptr<SubTexture2D>& texture);

		static void OnWindowResize(int width, int height);

	private:
		static void resetBatch();
		static void submitQuad(const QuadSpec& spec);
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

