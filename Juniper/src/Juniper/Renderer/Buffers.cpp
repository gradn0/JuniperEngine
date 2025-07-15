#include "pch.h"
#include "Buffers.h"

namespace Juniper {

	/**
	* Vertex Array
	*/

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_Id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	uint32_t VertexArray::GetIndexCount() const
	{
		if (!m_IndexBuffer)
		{
			JP_CORE_ERROR("VertexArray::GetIndexCount() called without an index buffer attached.");
			return 0;
		}
		return m_IndexBuffer->GetIndexCount();
	}

	void VertexArray::AttachVertexBuffer(const VertexBuffer& vertexBuffer)
	{
		m_VertexBuffer = &vertexBuffer;
	}

	void VertexArray::AttachIndexBuffer(const IndexBuffer& indexBuffer)
	{
		m_IndexBuffer = &indexBuffer;
		glVertexArrayElementBuffer(m_Id, indexBuffer.GetId());
	}

	void VertexArray::SetVertexLayout(const std::vector<VertexAttribute>& attributes)
	{
		// Compute the vertex buffer stride
		uint32_t stride = 0;
		for (const auto& attr : attributes)
			stride += attr.count * VertexAttribute::GetTypeSize(attr.type);

		// Set layout
		uint32_t offset = 0;
		for (uint32_t i = 0; i < attributes.size(); ++i)
		{
			const auto& attr = attributes.at(i);
			glEnableVertexArrayAttrib(m_Id, i);
			glVertexArrayAttribFormat(m_Id, i, attr.count, (uint32_t)attr.type, attr.normalised, offset);
			glVertexArrayAttribBinding(m_Id, i, 0);
			offset += attr.count * VertexAttribute::GetTypeSize(attr.type);
		}

		// Bind the vertex buffer
		if (!m_VertexBuffer)
		{
			JP_CORE_ERROR("Attempting to set vertex array layout without a vertex buffer attached. Be sure to call AttachVertexBuffer() before SetVertexLayout().");
			return;
		}
		glVertexArrayVertexBuffer(m_Id, 0, m_VertexBuffer->GetId(), 0, stride);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Id);
	}

	/**
	* Vertex Buffer
	*/

	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	{
		glCreateBuffers(1, &m_Id);
		glNamedBufferStorage(m_Id, size, data, GL_DYNAMIC_STORAGE_BIT);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	/**
	* Index Buffer
	*/

	IndexBuffer::IndexBuffer(const void* indices, uint32_t count) 
	{
		m_IndexCount = count;
		glCreateBuffers(1, &m_Id);
		glNamedBufferStorage(m_Id, count * sizeof(uint32_t), indices, GL_DYNAMIC_STORAGE_BIT);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	/**
	* Uniform buffer
	*/

	UniformBuffer::UniformBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_Id);
		glNamedBufferStorage(m_Id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void UniformBuffer::Bind(uint32_t slot) const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_Id);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size) const
	{
		glNamedBufferSubData(m_Id, 0, size, data);
	}
}
