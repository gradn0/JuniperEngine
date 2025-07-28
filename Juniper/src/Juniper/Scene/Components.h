#pragma once

#include "pch.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Juniper/Renderer/Texture.h"
#include "Juniper/Scene/Tilemap.h"

namespace Juniper {

	struct TagComponent
	{
		std::string Tag;
	};

	struct TransformComponent
	{
		glm::vec3 Translation{};
		glm::vec3 Rotation{};
		glm::vec3 Scale{ 1.0f };

		glm::mat4 GetTransform() const
		{
			glm::mat4 t = glm::translate(glm::mat4(1.0f), Translation);
			glm::mat4 r =
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1.0f, 0.0f, 0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y),{ 0.0f, 1.0f, 0.0f }) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f });
			glm::mat4 s = glm::scale(glm::mat4(1.0f), Scale);
			return t * r * s;
		}
	};

	struct SpriteComponent
	{
		glm::vec4 Color{1.0f};
		std::shared_ptr<Texture> Texture;
	};

	struct TilemapComponent
	{
		std::shared_ptr<Tilemap> Tilemap;
	};

}