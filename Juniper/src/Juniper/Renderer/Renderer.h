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
#include "Juniper/Scene/Tilemap.h"

namespace Juniper {

	struct Stats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t TextureSlotsUsed = 0;
	};

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
	};

	struct Capabilities
	{
		int TextureSlots = 32;
	};

	class Renderer
	{
	public:
		static void Init();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void SetDepthMask(bool enabled);

		static void BeginScene(const std::shared_ptr<OrthographicCamera>& camera, const std::shared_ptr<Shader> shader);
		static void EndScene();

		static void SubmitQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const std::shared_ptr<Texture>& texture);

		static void SubmitTilemap(const std::shared_ptr<Tilemap>& tilemap, const glm::vec3& translation);

		static void OnWindowResize(int width, int height);

		static Stats GetStats();
		static void ResetStats();

	private:
		static void resetBatch();
		static void flush();
		static void submitQuad(const std::array<glm::vec3, 4>& positions, const glm::vec4& color, const std::shared_ptr<Texture>& texture, const std::array<glm::vec2, 4>& texCoords);
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
