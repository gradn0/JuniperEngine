#pragma once

#include "pch.h"
#include "Juniper/Renderer/Texture.h"

namespace Juniper {

	class Tilemap
	{
		struct Tile {
			std::shared_ptr<Texture2D> Texture;
			std::array<glm::vec2, 4> TexCoords;
		};

		struct TilemapLayer
		{
			std::vector<Tile> TileRegistry;
			std::vector<int32_t> TileIndices;
		};

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

