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

	Texture::Texture() {}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Id);
	}

	void Texture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_Id);
	}

	Texture2D::Texture2D(const std::string& filepath, bool flip)
	{
		stbi_uc* data = LoadTexture(filepath, m_Width, m_Height, m_Channels, flip);
		if (!data)
			return;

		auto [internalFormat, format] = GetImageFormat(m_Channels);

		// Create texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
		glTextureStorage2D(m_Id, 1, internalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);

		// Set attributes
		glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateTextureMipmap(m_Id);
		stbi_image_free(data);
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
			JP_CORE_ERROR("Failed to load image: {0}", path);
		return data;
	}
}