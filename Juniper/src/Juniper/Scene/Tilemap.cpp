#include "pch.h"
#include "Juniper/Core/Log.h"
#include "Tilemap.h"
#include "tinyxml2.h"
#include "Juniper/Renderer/Texture.h"

namespace Juniper {

	struct Tilesets
	{
		std::vector<std::string> Sources;
		std::vector<uint32_t> FirstGids;
		std::vector<std::shared_ptr<Texture2D>> Textures;
		std::vector<uint32_t> Columns;
		std::vector<uint32_t> Rows;
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

		// Create global data
		uint32_t nextTextureIndex = 0;

		// Extract map data
		const tinyxml2::XMLElement* map = doc->FirstChildElement("map");
		map->FindAttribute("tilewidth")->QueryIntValue(&m_TileSize);

		// Get tileset data
		Tilesets tilesets;

		const tinyxml2::XMLElement* tileset = map->FirstChildElement("tileset");
		int firstGid = 1;
		do
		{
			// Load .tsx file
			const char* tsxSource = tileset->FindAttribute("source")->Value();
			std::filesystem::path tsxPath = baseDir / tsxSource;
			std::unique_ptr<tinyxml2::XMLDocument> tsxDoc = LoadXmlDoc(tsxPath.generic_string());

			// Add .png source to tileset
			auto ts = tsxDoc->FirstChildElement("tileset");
			auto image = ts->FirstChildElement("image");
			JP_CORE_ASSERT(ts, "Multiple images per tileset is not supported yet");

			const char* source = image->FindAttribute("source")->Value();
			std::filesystem::path imagePath = baseDir / source;
			imagePath = std::filesystem::weakly_canonical(imagePath);
			tilesets.Sources.emplace_back(imagePath.string());

			// Add column count
			int cols = 0;
			ts->FindAttribute("columns")->QueryIntValue(&cols);
			tilesets.Columns.push_back(cols);

			int size = 0;
			ts->FindAttribute("tilecount")->QueryIntValue(&size);
			tilesets.Rows.push_back(size / cols);

			// Add first gid to tileset
			tileset->FindAttribute("firstgid")->QueryIntValue(&firstGid);
			tilesets.FirstGids.push_back(firstGid);
		}
		while ((tileset = tileset->NextSiblingElement("tileset")));
		
		tilesets.Textures.resize(tilesets.Sources.size());

		// Extract layer data
		std::unordered_map<uint32_t, uint32_t> indexMap; // Mapping the raw indices to useful indices into our tile subtextures
		const tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
		do
		{
			TilemapLayer tilemapLayer;

			// TODO: Only needs to happen once
			layer->FindAttribute("width")->QueryIntValue(&m_Width);
			layer->FindAttribute("height")->QueryIntValue(&m_Height);

			const char* text = layer->FirstChildElement("data")->GetText();

			int tileCount = m_Width * m_Height;

			std::string str;
			std::stringstream stream(text);
			while (stream.good())
			{
				std::getline(stream, str, ',');
				int index = std::stoi(str);
				int mappedIndex;

				// Handle blank tiles (gid = 0)
				if (index == 0)
				{
					tilemapLayer.TextureIndices.push_back(-1);
					continue;
				}

				auto it = indexMap.find(index);
				if (it != indexMap.end())
				{
					mappedIndex = it->second;
				}
				else
				{
					size_t sourceIndex = 0;
					size_t baseIndex = 1; // Base index for the tileset

					// Get the tilemap source index
					for (size_t i = 0; i < tilesets.FirstGids.size(); ++i)
					{
						uint32_t gidBegin = tilesets.FirstGids[i];
						uint32_t gidEnd = (i + 1 < tilesets.FirstGids.size()) ? tilesets.FirstGids[i + 1] : UINT32_MAX;

						if (index >= gidBegin && index < gidEnd)
						{
							sourceIndex = i;
							baseIndex = gidBegin;
							break;
						}
					}

					// Create a new tileset texture if needed
					if (!tilesets.Textures[sourceIndex])
						tilesets.Textures[sourceIndex] = std::make_shared<Texture2D>(tilesets.Sources[sourceIndex]);

					// Create new index mapping
					mappedIndex = static_cast<uint32_t>(tilemapLayer.Textures.size());
					indexMap[index] = mappedIndex;

					// Create subtexture
					int offset = index - baseIndex;
					int cols = tilesets.Columns[sourceIndex];
					int rows = tilesets.Rows[sourceIndex];

					auto subTexture = std::make_shared<SubTexture2D>(
						tilesets.Textures[sourceIndex],
						glm::vec2{ offset % cols, rows - (offset / cols) - 1 },
						glm::vec2{ 1.0f },
						static_cast<float>(m_TileSize)
					);

					// Append subtexture
					tilemapLayer.Textures.emplace_back(subTexture);
				}


				tilemapLayer.TextureIndices.push_back(mappedIndex);
			}

			m_Layers.push_back(tilemapLayer);
		}
		while (layer = layer->NextSiblingElement("layer"));
	}

}
