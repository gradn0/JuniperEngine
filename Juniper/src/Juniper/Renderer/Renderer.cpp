#include "pch.h"
#include "GLFW/glfw3.h"
#include "Juniper/Core/Log.h"
#include "Renderer.h"
#include "tinyxml2.h"

namespace Juniper {

	constexpr uint32_t MaxQuads = 5000;
	constexpr uint32_t MaxVertices = MaxQuads * 4;
	constexpr uint32_t MaxIndices = MaxQuads * 6;
	constexpr uint32_t MaxTextures = 32;
	constexpr std::array<glm::vec2, 4> DefaultTexCoords = {
		glm::vec2{0.0f, 0.0f},
		glm::vec2{1.0f, 0.0f},
		glm::vec2{1.0f, 1.0f},
		glm::vec2{0.0f, 1.0f}
	};
	constexpr std::array<glm::vec4, 4> DefaultVertexPositions = {
		glm::vec4{-0.5f, -0.5f, 0.0f, 1.0f},
		glm::vec4{0.5f, -0.5f, 0.0f, 1.0f},
		glm::vec4{0.5f, 0.5f, 0.0f, 1.0f},
		glm::vec4{-0.5f, 0.5f, 0.0f, 1.0f}
	};
	constexpr uint32_t indexPattern[6] = { 0, 1, 2, 2, 3, 0 };

	struct Data
	{
		glm::mat4 ViewProjection;
		std::shared_ptr<VertexArray> Vao;
		std::shared_ptr<Shader> Shader;
		std::shared_ptr<Texture> DefaultTexture;

		QuadVertex Vertices[MaxVertices];
		uint32_t Indices[MaxIndices];
		std::shared_ptr<Texture> Textures[MaxTextures];
		int32_t slots[MaxTextures];

		uint32_t VertexPtr = 0;
		uint32_t IndexPtr = 0;
		uint32_t TexturesPtr = 0;

		Stats Stats;
	};

	static Data s_Data;
	static Capabilities s_Capabilities;

	void Renderer::Init()
	{
		// Initialise opengl
		JP_CORE_VERIFY(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load OpenGL");
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(messageCallback, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Query capabilities
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_Capabilities.TextureSlots);

		// Init data
		auto vbo = std::make_shared<VertexBuffer>(static_cast<uint32_t>(MaxVertices * sizeof(QuadVertex)));
		auto ibo = std::make_shared<IndexBuffer>(MaxIndices);

		s_Data.Vao = std::make_shared<VertexArray>();
		s_Data.Vao->AttachVertexBuffer(vbo);
		s_Data.Vao->AttachIndexBuffer(ibo);
		s_Data.Vao->SetVertexLayout({
			{ 3, AttributeType::Float, false },
			{ 4, AttributeType::Float, false },
			{ 2, AttributeType::Float, false },
			{ 1, AttributeType::Float, false }
		});

		s_Data.DefaultTexture = std::make_shared<Texture2D>(glm::u8vec4(255));
		s_Data.Textures[0] = s_Data.DefaultTexture;

        // Initialise sampler array to be sent to the shader
		for (int32_t i = 0; i < MaxTextures; i++)
			s_Data.slots[i] = i;
	}

	void Renderer::BeginScene(const OrthographicCamera& camera, const std::shared_ptr<Shader> shader)
	{
		s_Data.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.Shader = shader;
		resetBatch();

		s_Data.Shader->Bind();
		s_Data.Shader->setUniformMat4("u_ViewProjection", s_Data.ViewProjection);
		s_Data.Shader->setUniformMat4("u_Model", glm::mat4(1.0f));
		s_Data.Shader->setUniformArrayi("u_Textures", static_cast<int>(MaxTextures), s_Data.slots);
	}

	void Renderer::EndScene()
	{
		flush();
	}

	void Renderer::SubmitTilemap(const std::shared_ptr<Tilemap>& tilemap, const glm::vec3& translation)
	{
		int mapHeight = tilemap->GetHeight();
		auto& layers = tilemap->GetLayers();

		std::shared_ptr<Texture> currentTexture = nullptr;

		for (int i = 0; i < layers.size(); ++i)
		{
			auto& layer = layers[i];
			for (int j = 0; j < layer.TileIndices.size(); ++j)
			{
				auto& index = layer.TileIndices[j];
				if (index == -1) continue;

				auto& tile = layer.TileRegistry[index];

                // TODO: Fix the root issue (texture switching causes artifacts)
				if (currentTexture && tile.Texture->GetTexture() != currentTexture->GetTexture()) {
					flush();
					resetBatch();
				}
				currentTexture = tile.Texture;

				float x = static_cast<float>(j % tilemap->GetWidth());
				float y = static_cast<float>(mapHeight - (j / tilemap->GetWidth()) - 1);

				auto& position = glm::vec3{ x, y, 0.0f } + translation;
				auto& size = glm::vec2{ 1.0f, 1.0f };
                std::array<glm::vec3, 4> positions = {
                    position,
                    { position.x + size.x, position.y, position.z },
                    { position.x + size.x, position.y + size.y, position.z },
                    { position.x, position.y + size.y, position.z },
                };

				submitQuad(
					positions,
					glm::vec4(1.0f),
					tile.Texture,
					tile.TexCoords
				);
			}
		}
	}

	void Renderer::SubmitQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture)
	{
		auto& tex = texture ? texture : s_Data.DefaultTexture;
		submitQuad(
			{
				position,
				{ position.x + size.x, position.y, position.z },
				{ position.x + size.x, position.y + size.y, position.z },
				{ position.x, position.y + size.y, position.z },
			},
			color,
			tex,
			tex->GetTexCoords()
		);
	}

	void Renderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const std::shared_ptr<Texture>& texture)
	{
		auto& tex = texture ? texture : s_Data.DefaultTexture;
		std::array<glm::vec3, 4> positions = {};
		for (size_t i = 0; i < DefaultVertexPositions.size(); ++i)
			positions[i] = transform * DefaultVertexPositions[i];

		submitQuad(
			positions,
			color,
			tex,
			tex->GetTexCoords()
		);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetDepthMask(bool enabled)
	{
		glDepthMask(enabled);
	}

	void Renderer::OnWindowResize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	Stats Renderer::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer::ResetStats()
	{
		s_Data.Stats = Stats{};
	}
}

namespace Juniper {

	void Renderer::submitQuad(const std::array<glm::vec3, 4>& positions, const glm::vec4& color, const std::shared_ptr<Texture>& texture, const std::array<glm::vec2, 4>& texCoords)
	{
		s_Data.Stats.QuadCount++;

		int existingSlot = -1;
		float textureSlot = 0.0f;

		// Check if texture has already been used this batch
		for (uint32_t i = 0; i < s_Data.TexturesPtr; i++)
		{
			if (s_Data.Textures[i] == texture)
			{
				existingSlot = i;
				break;
			}
		}

		// Check whether an early flush is required
		bool needNewTextureSlot = (existingSlot == -1);
		bool textureLimitReached = s_Data.TexturesPtr >= std::min<uint32_t>(static_cast<uint32_t>(s_Capabilities.TextureSlots), MaxTextures);
		bool bufferLimitReached = s_Data.VertexPtr + 4 > MaxVertices || s_Data.IndexPtr + 6 > MaxIndices;

		if ((needNewTextureSlot && textureLimitReached) || bufferLimitReached)
		{
			flush();
			resetBatch();
			existingSlot = -1;
		}

		if (existingSlot == -1)
		{
			s_Data.Stats.TextureSlotsUsed++;
			s_Data.Textures[s_Data.TexturesPtr] = texture;
			textureSlot = static_cast<float>(s_Data.TexturesPtr++);
		}
		else
		{
			textureSlot = static_cast<float>(existingSlot);
		}

		QuadVertex vertex{};
		auto baseVertex = static_cast<uint32_t>(s_Data.VertexPtr);

		// Append vertices
		for (size_t i = 0; i < 4; i++)
		{
			vertex.Position = positions[i];
			vertex.Color = color;
			vertex.TexCoords = texCoords[i];
			vertex.TexIndex = textureSlot;
			s_Data.Vertices[s_Data.VertexPtr++] = vertex;
		}

		// Append indices
		for (size_t i = 0; i < 6; i++)
			s_Data.Indices[s_Data.IndexPtr + i] = baseVertex + indexPattern[i];

		s_Data.IndexPtr += 6;
	}

	void Renderer::resetBatch()
	{
		s_Data.VertexPtr = 0;
		s_Data.IndexPtr = 0;
		s_Data.TexturesPtr = 1;
	}

	void Renderer::flush()
	{
		s_Data.Stats.DrawCalls++;

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TexturesPtr; i++)
			s_Data.Textures[i]->Bind(i);

		// Set buffer data
		s_Data.Vao->GetVertexBuffer()->SetData(s_Data.Vertices, s_Data.VertexPtr * sizeof(QuadVertex));
		s_Data.Vao->GetIndexBuffer()->SetData(s_Data.Indices, s_Data.IndexPtr * sizeof(uint32_t));

		drawIndexed(*s_Data.Vao, s_Data.IndexPtr, *s_Data.Shader);
	}

	void Renderer::drawIndexed(const VertexArray& vertexArray, size_t indexCount, const Shader& shader)
	{
		vertexArray.Bind();
		shader.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<int>(indexCount), GL_UNSIGNED_INT, 0);
	}

	void Renderer::drawIndexed(const VertexArray& vertexArray, const Shader& shader)
	{
		drawIndexed(vertexArray, vertexArray.GetIndexCount(), shader);
	}

	void GLAPIENTRY Renderer::messageCallback(
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
