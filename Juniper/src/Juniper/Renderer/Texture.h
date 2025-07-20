#pragma once

#include "pch.h"
#include "glm/glm.hpp"
#include "Juniper/Core/Core.h"

namespace Juniper {

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		void Bind(uint32_t slot = 0) const;

		inline uint32_t GetId() const { return m_Id;  }
		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		bool operator==(const Texture& other) const { return m_Id == other.GetId(); }

	protected:
		uint32_t m_Id = 0;
		int m_Channels = 0;
		int m_Width = 0, m_Height = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& filepath, bool flip = true);
		Texture2D(glm::u8vec4 color);
	};

	class SubTexture2D
	{
	public:
		SubTexture2D(const std::shared_ptr<Texture2D>& parent, glm::vec2 offset, glm::vec2 size, float spriteSize);
		~SubTexture2D() = default;

		std::shared_ptr<Texture2D> GetTexture() const { return m_Parent; }
		std::array<glm::vec2, 4> GetTextureCoords() const { return m_TexCoords; }

	private:
		std::shared_ptr<Texture2D> m_Parent;
		std::array<glm::vec2, 4> m_TexCoords;
	};

}