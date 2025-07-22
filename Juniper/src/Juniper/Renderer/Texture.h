#pragma once

#include "pch.h"
#include "glm/glm.hpp"
#include "Juniper/Core/Core.h"

namespace Juniper {

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual uint32_t GetId() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual Texture* GetTexture() { return this; }
		virtual std::array<glm::vec2, 4> GetTexCoords() const {
			return {{
				{0.0f, 0.0f},
				{1.0f, 0.0f},
				{1.0f, 1.0f},
				{0.0f, 1.0f}
			}};
		}

		bool operator==(const Texture& other) const { return GetId() == other.GetId(); }
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& filepath, bool flip = true);
		Texture2D(glm::u8vec4 color);
		~Texture2D();

		void Bind(uint32_t slot = 0) const override;

		uint32_t GetId() const override { return m_Id; }
		int GetWidth() const override { return m_Width; }
		int GetHeight() const override { return m_Height; }

	private:
		uint32_t m_Id = 0;
		int m_Channels = 0;
		int m_Width = 0, m_Height = 0;
	};

	// This class doesn't own any opengl state. It holds a reference to a parent opengl texture and some texture coords
	// However, it does expose the same methods for convenience
	class SubTexture2D : public Texture
	{
	public:
		SubTexture2D(const std::shared_ptr<Texture2D>& parent, glm::vec2 offset, glm::vec2 size, float spriteSize);
		~SubTexture2D() = default;

		void Bind(uint32_t slot = 0) const override;

		uint32_t GetId() const override { return m_Parent->GetId(); }
		int GetWidth() const override { return m_Parent->GetWidth(); }
		int GetHeight() const override { return m_Parent->GetHeight(); }
		Texture* GetTexture() override { return m_Parent.get(); }
		std::array<glm::vec2, 4> GetTexCoords() const override { return m_TexCoords; }

	private:
		std::shared_ptr<Texture2D> m_Parent;
		std::array<glm::vec2, 4> m_TexCoords;
	};

}