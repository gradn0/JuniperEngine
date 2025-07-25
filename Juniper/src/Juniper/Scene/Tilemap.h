#pragma once

#include "pch.h"
#include "Juniper/Renderer/Texture.h"

namespace Juniper {

	struct TilemapLayer
	{
		std::vector<std::shared_ptr<SubTexture2D>> Textures; // TODO: Probably don't want a vector of ptrs here
		std::vector<int> TextureIndices;
	};

	class Tilemap
	{
	public:
		Tilemap(const std::string& filepath);
		~Tilemap() = default;

		const std::vector<TilemapLayer>& GetLayers() const { return m_Layers; }
		int GetTileSize() const { return m_TileSize; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		int m_Height = 0;
		int m_Width = 0;
		int m_TileSize = 0;
		std::vector<TilemapLayer> m_Layers;
	};

}

