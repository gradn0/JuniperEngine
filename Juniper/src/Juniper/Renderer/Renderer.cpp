#include "pch.h"
#include "GLFW/glfw3.h"
#include "Juniper/Core/Log.h"
#include "Renderer.h"

namespace Juniper {
	constexpr uint32_t MaxQuads = 1000;
	constexpr uint32_t MaxVertices = MaxQuads * 4;
	constexpr uint32_t MaxIndices = MaxQuads * 6;
	constexpr uint32_t MaxTextures = 32;
	constexpr std::array<glm::vec2, 4> DefaultTexCoords = {
		glm::vec2{0.0f, 0.0f},
		glm::vec2{1.0f, 0.0f},
		glm::vec2{1.0f, 1.0f},
		glm::vec2{0.0f, 1.0f}
	};

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
	};

	static Data s_Data;
	static Capabilities s_Capabilities;

	void Renderer::Init()
	{
		// Initialise opengl
		JP_CORE_VERIFY(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load OpenGL");
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(messageCallback, 0);

		glEnable(GL_DEPTH_TEST);

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

	void Renderer::SubmitQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, const std::shared_ptr<Texture>& texture)
	{
		auto& tex = texture ? texture : s_Data.DefaultTexture;
		submitQuad({
			position,
			color,
			size,
			tex->GetTexCoords(),
			tex
		});
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

	void Renderer::OnWindowResize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}

// Internal
namespace Juniper {

	void Renderer::submitQuad(const QuadSpec& spec)
	{
		int existingSlot = -1;
		float textureSlot = 0.0f;

		// Check if texture has already been used this batch
		for (uint32_t i = 0; i < s_Data.TexturesPtr; i++)
		{
			if (s_Data.Textures[i] == spec.Texture)
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
			s_Data.Textures[s_Data.TexturesPtr] = spec.Texture;
			textureSlot = static_cast<float>(s_Data.TexturesPtr++);
		}
		else
		{
			textureSlot = static_cast<float>(existingSlot);
		}

		QuadVertex vertex{};
		auto baseVertex = static_cast<uint32_t>(s_Data.VertexPtr);

		// Bottom Left
		vertex.Position = spec.Position;
		vertex.Color = spec.Color;
		vertex.TexCoords = spec.TexCoords[0];
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		// Bottom Right
		vertex.Position = spec.Position + glm::vec3(spec.Size.x, 0.0f, 0.0f);
		vertex.Color = spec.Color;
		vertex.TexCoords = spec.TexCoords[1];
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		// Top Right
		vertex.Position = spec.Position + glm::vec3(spec.Size.x, spec.Size.y, 0.0f);
		vertex.Color = spec.Color;
		vertex.TexCoords = spec.TexCoords[2];
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		// Top Left
		vertex.Position = spec.Position + glm::vec3(0.0f, spec.Size.y, 0.0f);
		vertex.Color = spec.Color;
		vertex.TexCoords = spec.TexCoords[3];
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		s_Data.Indices[s_Data.IndexPtr + 0] = baseVertex + 0;
		s_Data.Indices[s_Data.IndexPtr + 1] = baseVertex + 1;
		s_Data.Indices[s_Data.IndexPtr + 2] = baseVertex + 2;
		s_Data.Indices[s_Data.IndexPtr + 3] = baseVertex + 2;
		s_Data.Indices[s_Data.IndexPtr + 4] = baseVertex + 3;
		s_Data.Indices[s_Data.IndexPtr + 5] = baseVertex + 0;

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
