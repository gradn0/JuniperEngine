#include "pch.h"
#include "Primitives.h"

namespace Juniper { namespace Primitives {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
	};

	std::shared_ptr<VertexArray> Quad(glm::vec4 color)
	{
		std::vector<Vertex> vertices = {
			{{ -0.5f, -0.5f, 0.0f }, color, { 0.0f, 0.0f }},
			{{  0.5f, -0.5f, 0.0f }, color, { 1.0f, 0.0f }},
			{{  0.5f,  0.5f, 0.0f }, color, { 1.0f, 1.0f }},
			{{ -0.5f,  0.5f, 0.0f }, color, { 0.0f, 1.0f }},
		};

		std::vector<uint32_t> indices = {
			0, 1, 2,
			2, 3, 0
		};

		auto vao = std::make_shared<VertexArray>();
		auto vbo = std::make_shared<VertexBuffer>(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Vertex)));
		auto ibo = std::make_shared<IndexBuffer>(indices.data(), static_cast<uint32_t>(indices.size()));

		vao->AttachVertexBuffer(vbo);
		vao->AttachIndexBuffer(ibo);
		vao->SetVertexLayout({
			{ 3, AttributeType::Float, false },
			{ 4, AttributeType::Float, false },
			{ 2, AttributeType::Float, false }
		});

		return vao;
	}

}}