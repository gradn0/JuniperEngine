#pragma once

#include "pch.h"
#include "glad/glad.h"
#include "Juniper/Core/Core.h"
#include "Juniper/Core/Log.h"

namespace Juniper {

	enum class AttributeType
	{
		None = 0, Float = GL_FLOAT, Uint = GL_UNSIGNED_INT, Ubyte = GL_UNSIGNED_BYTE
	};

	struct VertexAttribute {
		uint32_t count;
		AttributeType type;
		uint32_t normalised;

		static uint32_t GetTypeSize(AttributeType type)
		{
			switch (type)
			{
				case AttributeType::Float:	return 4;
				case AttributeType::Uint:	return 4;
				case AttributeType::Ubyte:	return 1;
			}
			JP_CORE_ASSERT(false, "Unknown vertex type");
			return 0;
		}
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, uint32_t size);
		VertexBuffer(uint32_t size);

		void SetData(const void* data, uint32_t size) const;

		~VertexBuffer();
		inline uint32_t GetId() const { return m_Id; };

	private:
		uint32_t m_Id = 0;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const void* indices, uint32_t count);
		IndexBuffer( uint32_t count);
		~IndexBuffer();

		void SetData(const void* data, uint32_t size) const;

		inline uint32_t GetId() const { return m_Id; };
		inline uint32_t GetIndexCount() const { return m_IndexCount; };

	private:
		uint32_t m_Id = 0;
		uint32_t m_IndexCount = 0;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;

		inline uint32_t GetId() const { return m_Id; };
		uint32_t GetIndexCount() const;

		inline std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
		inline std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

		void AttachVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void AttachIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
		void SetVertexLayout(const std::vector<VertexAttribute>& attributes);

	private:
		uint32_t m_Id = 0;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

	class UniformBuffer
	{
	public:
		UniformBuffer(uint32_t size);
		~UniformBuffer();

        inline uint32_t GetId() const { return m_Id; };
        
		void Bind(uint32_t slot = 0) const;
		void SetData(const void* data, uint32_t size) const;

	private:
		uint32_t m_Id = 0;
	};

}