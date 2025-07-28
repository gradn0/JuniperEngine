#include "pch.h"
#include "Juniper/Core/Log.h"
#include "Tilemap.h"
#include "tinyxml2.h"
#include "Juniper/Renderer/Texture.h"

namespace Juniper {

	struct Tilesets
	{
		std::vector<uint32_t> FirstGids;
		std::vector<std::shared_ptr<Texture2D>> Textures;
		std::vector<uint32_t> Columns;
		std::vector<uint32_t> Rows;
		uint32_t Count = 0;
	};

	static std::unique_ptr<tinyxml2::XMLDocument> LoadXmlDoc(const std::string& filepath)
	{
		auto doc = std::make_unique<tinyxml2::XMLDocument>();

		auto err = doc->LoadFile(filepath.c_str());
		if (err)
			JP_CORE_ASSERT(false, "Failed to load XML document");

		return doc;
	}

	Tilemap::Tilemap(const std::string& filepath)
	{
		std::filesystem::path fullpath(filepath);
		auto& baseDir = fullpath.remove_filename();

		// Load XML document
		std::unique_ptr<tinyxml2::XMLDocument> doc = LoadXmlDoc(filepath);

		// Get map data
		const tinyxml2::XMLElement* map = doc->FirstChildElement("map");
		map->FindAttribute("tilewidth")->QueryIntValue(&m_TileSize);

		// Get tilesets data
		Tilesets tilesets;
		const tinyxml2::XMLElement* tileset = map->FirstChildElement("tileset");
		do
		{
			// Load .tsx file
			const char* tsxSource = tileset->FindAttribute("source")->Value();
			std::filesystem::path tsxPath = baseDir / tsxSource;
			std::unique_ptr<tinyxml2::XMLDocument> tsxDoc = LoadXmlDoc(tsxPath.generic_string());

			// Add image path to tilesets
			auto ts = tsxDoc->FirstChildElement("tileset");
			auto image = ts->FirstChildElement("image");
			JP_CORE_ASSERT(ts, "Multiple images per tileset is not supported yet");

			const char* source = image->FindAttribute("source")->Value();
			std::filesystem::path imagePath = baseDir / source;
			imagePath = std::filesystem::weakly_canonical(imagePath);

			// Add tile data to tilesets
			int cols = 0;
			ts->FindAttribute("columns")->QueryIntValue(&cols);
			tilesets.Columns.push_back(cols);

			int size = 0;
			ts->FindAttribute("tilecount")->QueryIntValue(&size);
			tilesets.Rows.push_back(size / cols);

			int firstGid = 1;
			tileset->FindAttribute("firstgid")->QueryIntValue(&firstGid);
			tilesets.FirstGids.push_back(firstGid);

			// Load textures and store in tilesets
			tilesets.Textures.emplace_back(std::make_shared<Texture2D>(imagePath.string()));

			tilesets.Count++;
		}
		while ((tileset = tileset->NextSiblingElement("tileset")));
		
		// Extract layer data
		std::unordered_map<uint32_t, uint32_t> gidToTileIndex;
		const tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
		do
		{
			TilemapLayer tilemapLayer;

			layer->FindAttribute("width")->QueryIntValue(&m_Width);
			layer->FindAttribute("height")->QueryIntValue(&m_Height);

			const char* data = layer->FirstChildElement("data")->GetText();

			int tileCount = m_Width * m_Height;

			std::string gidStr;
			std::stringstream stream(data);
			while (stream.good())
			{
				std::getline(stream, gidStr, ',');
				uint32_t gid = static_cast<uint32_t>(std::stoi(gidStr));
				int tileIndex;

				// Handle blank/invalid tiles
				if (gid <= 0)
				{
					tilemapLayer.TileIndices.push_back(-1);
					continue;
				}

				auto it = gidToTileIndex.find(gid);
				if (it != gidToTileIndex.end())
				{
					// Tile exists
					tileIndex = it->second;
				}
				else
				{
					// Tile needes to be added to registry
					uint32_t tilesetIndex = 0;
					uint32_t baseGid = 1;

					// Determine which tileset to use based on gid
					for (uint32_t i = 0; i < tilesets.Count; ++i)
					{
						uint32_t gidBegin = tilesets.FirstGids[i];
						uint32_t gidEnd = (i + 1 < tilesets.Count) ? tilesets.FirstGids[i + 1] : UINT32_MAX;

						if (gid >= gidBegin && gid < gidEnd)
						{
							tilesetIndex = i;
							baseGid = gidBegin;
							break;
						}
					}

					// Create tile
					int tilesetOffset = gid - baseGid;
					int cols = tilesets.Columns[tilesetIndex];
					int rows = tilesets.Rows[tilesetIndex];

					glm::vec2 parentSize = {
						static_cast<float>(cols * m_TileSize),
						static_cast<float>(rows * m_TileSize)
					};

					glm::vec2 offsetTiles = {
						static_cast<float>(tilesetOffset % cols),
						static_cast<float>(rows - (tilesetOffset / cols) - 1)
					};

					glm::vec2& sizeTiles = glm::vec2{ 1.0f };

					tilemapLayer.TileRegistry.emplace_back(Tile{
						tilesets.Textures[tilesetIndex],
						Texture::GenerateTexCoords(parentSize, offsetTiles, sizeTiles, static_cast<float>(m_TileSize))
					});

					tileIndex = static_cast<uint32_t>(tilemapLayer.TileRegistry.size() - 1);
					gidToTileIndex[gid] = tileIndex;
				}


				tilemapLayer.TileIndices.push_back(tileIndex);
			}

			m_Layers.push_back(tilemapLayer);
		}
		while (layer = layer->NextSiblingElement("layer"));
	}

}
