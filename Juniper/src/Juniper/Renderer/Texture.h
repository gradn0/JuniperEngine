#pragma once

#include "pch.h"
#include "Juniper/Core/Core.h"

namespace Juniper {

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		void Bind(uint32_t slot = 0) const;
		inline uint32_t GetId() const { return m_Id;  }

	protected:
		uint32_t m_Id = 0;
		int m_Channels = 0;
		int m_Width = 0, m_Height = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& filepath, bool flip = true);
	};

}