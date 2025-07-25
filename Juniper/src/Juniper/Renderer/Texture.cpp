#include "pch.h"
#include <glad/glad.h>
#include "stb_image.h"
#include "Juniper/Core/Log.h"
#include "Texture.h"

namespace Juniper {

	static std::pair<uint32_t, uint32_t> GetImageFormat(int channels);
	static stbi_uc* LoadTexture(const std::string& path, int& width, int& height, int& channels, bool flip);

}

namespace Juniper {

	// TODO: Make this api nicer
	std::array<glm::vec2, 4> Texture::GenerateTexCoords(glm::vec2 parentSizePix, glm::vec2 childSizePrim, float primitiveSizePix, glm::vec2 offsetPrim)
	{
		glm::vec2 min{
			(offsetPrim.x * primitiveSizePix) / static_cast<float>(parentSizePix.x),
			(offsetPrim.y * primitiveSizePix) / static_cast<float>(parentSizePix.y)
		};

		glm::vec2 max{
			((offsetPrim.x + childSizePrim.x) * primitiveSizePix) / static_cast<float>(parentSizePix.x),
			((offsetPrim.y + childSizePrim.y) * primitiveSizePix) / static_cast<float>(parentSizePix.y)
		};

		return {
			glm::vec2{ min.x, min.y },
			glm::vec2{ max.x, min.y },
			glm::vec2{ max.x, max.y },
			glm::vec2{ min.x, max.y }
		};
	}

	Texture2D::Texture2D(const std::string& filepath, bool flip)
	{
		stbi_uc* data = LoadTexture(filepath, m_Width, m_Height, m_Channels, flip);
		if (!data)
			return;

		JP_CORE_TRACE("Loading texture: \"{0}\" ({1}x{2})", filepath, m_Width, m_Height);

		auto [internalFormat, format] = GetImageFormat(m_Channels);

		// Create texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
		glTextureStorage2D(m_Id, 1, internalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);

		// Set attributes
		glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenerateTextureMipmap(m_Id);
		stbi_image_free(data);
	}

	Texture2D::Texture2D(glm::u8vec4 color)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
		glTextureStorage2D(m_Id, 1, GL_RGBA8, 1, 1);

		uint8_t pixel[4] = { color.r, color.g, color.b, color.a };

		glTextureSubImage2D(m_Id, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

		glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Id);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_Id);
	}

	SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& parent, glm::vec2 offset, glm::vec2 size, float spriteSize)
		: m_Parent(parent)
	{
		m_TexCoords = GenerateTexCoords({ parent->GetWidth(), parent->GetHeight() }, size, spriteSize, offset);
	}
    
	void SubTexture2D::Bind(uint32_t slot) const
	{
		m_Parent->Bind(slot);
	}
}

namespace Juniper {

	static std::pair<uint32_t, uint32_t> GetImageFormat(int channels)
	{
		switch (channels)
		{
			case 1: return { GL_R8, GL_RED };
			case 3: return { GL_RGB8, GL_RGB };
			case 4: return { GL_RGBA8, GL_RGBA };
		default:
			JP_CORE_ERROR("Unsupported channel count: {0}", channels);
			return { GL_RGBA8, GL_RGBA };
		}
	}

	static stbi_uc* LoadTexture(const std::string& path, int& width, int& height, int& channels, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data)
			JP_CORE_ERROR("Failed to load image {0}: {1}", path, stbi_failure_reason());
		return data;
	}
}