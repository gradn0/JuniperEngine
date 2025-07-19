#include "pch.h"
#include "GLFW/glfw3.h"
#include "Juniper/Core/Log.h"
#include "Renderer.h"

namespace Juniper {

	constexpr size_t MaxQuads = 1000;
	constexpr size_t MaxVertices = MaxQuads * 4;
	constexpr size_t MaxIndices = MaxQuads * 6;
	constexpr size_t MaxTextures = 32;

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
	};

	struct Data
	{
		glm::mat4 ViewProjection;
		std::shared_ptr<VertexArray> Vao;
		std::shared_ptr<Shader> Shader;
		std::shared_ptr<Texture2D> DefaultTexture;

		QuadVertex Vertices[MaxVertices];
		uint32_t Indices[MaxIndices];
		std::shared_ptr<Texture2D> Textures[MaxTextures];
		int32_t slots[MaxTextures];

		size_t VertexPtr;
		size_t IndexPtr;
		size_t TexturesPtr;
	};

	static Data s_Data;

	void Renderer::Init()
	{
		// Initialise opengl
		JP_CORE_VERIFY(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load OpenGL");
		
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		glEnable(GL_DEPTH_TEST);

		// Init data
		auto vbo = std::make_shared<VertexBuffer>(MaxVertices * sizeof(QuadVertex));
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
		for (size_t i = 0; i < MaxTextures; i++)
			s_Data.slots[i] = i;
	}

	void Renderer::BeginScene(const OrthographicCamera& camera, const std::shared_ptr<Shader> shader)
	{
		s_Data.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.Shader = shader;

		s_Data.VertexPtr = 0;
		s_Data.IndexPtr = 0;
		s_Data.TexturesPtr = 1;

		s_Data.Shader->Bind();
		s_Data.Shader->setUniformMat4("u_ViewProjection", s_Data.ViewProjection);
		s_Data.Shader->setUniformMat4("u_Model", glm::mat4(1.0f));
		s_Data.Shader->setUniformArrayi("u_Textures", MaxTextures, s_Data.slots);
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::Flush()
	{
        // Bind shaders
		for (size_t i = 0; i < s_Data.TexturesPtr; i++)
			s_Data.Textures[i]->Bind(i);

        // Set buffer data
		s_Data.Vao->GetVertexBuffer()->SetData(s_Data.Vertices, s_Data.VertexPtr * sizeof(QuadVertex));
		s_Data.Vao->GetIndexBuffer()->SetData(s_Data.Indices, s_Data.IndexPtr * sizeof(uint32_t));

		DrawIndexed(*s_Data.Vao, s_Data.IndexPtr, *s_Data.Shader);
	}

	void Renderer::SubmitQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
	{
		SubmitQuad(position, size, color, s_Data.DefaultTexture);
	}

	void Renderer::SubmitQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color, const std::shared_ptr<Texture2D>& texture)
	{
		float textureSlot = 0;
		bool found = false;
		for (size_t i = 0; i < s_Data.TexturesPtr; i++)
		{
			if (s_Data.Textures[i] == texture)
			{
				textureSlot = static_cast<float>(i);
				found = true;
				break;
			}
		}

		if (!found)
		{
			s_Data.Textures[s_Data.TexturesPtr] = texture;
			textureSlot = s_Data.TexturesPtr++;
		}

		QuadVertex vertex{};
		auto baseVertex = static_cast<uint32_t>(s_Data.VertexPtr);

		// Bottom Left
		vertex.Position = { position.x, position.y, position.z };
		vertex.Color = color;
		vertex.TexCoords = { 0.0f, 0.0f };
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		// Bottom Right
		vertex.Position = { position.x + size.x, position.y, position.z };
		vertex.Color = color;
		vertex.TexCoords = { 1.0f, 0.0f };
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		// Top Right
		vertex.Position = { position.x + size.x, position.y + size.y, position.z };
		vertex.Color = color;
		vertex.TexCoords = { 1.0f, 1.0f };
		vertex.TexIndex = textureSlot;
		s_Data.Vertices[s_Data.VertexPtr++] = vertex;

		// Top Left
		vertex.Position = { position.x, position.y + size.y, position.z };
		vertex.Color = color;
		vertex.TexCoords = { 0.0f, 1.0f };
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

	void Renderer::DrawIndexed(const VertexArray& vertexArray, size_t indexCount, const Shader& shader)
	{
		vertexArray.Bind();
		shader.Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}

	void Renderer::DrawIndexed(const VertexArray& vertexArray, const Shader& shader)
	{
		DrawIndexed(vertexArray, vertexArray.GetIndexCount(), shader);
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
