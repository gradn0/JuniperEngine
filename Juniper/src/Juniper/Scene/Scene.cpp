#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Juniper/Renderer/Renderer.h"

namespace Juniper {

	void Scene::OnUpdate(float dt)
	{
		// Render tilemaps
		auto& tilemapView = GetView<TilemapComponent, TransformComponent>();
		for (auto [entity, tilemap, transform] : tilemapView.each())
			Renderer::SubmitTilemap(tilemap.Tilemap, transform.Translation);

		// Render sprites
		auto& spriteView = GetView<SpriteComponent, TransformComponent>();
		for (auto [entity, sprite, transform] : spriteView.each())
			Renderer::SubmitQuad(transform.GetTransform(), sprite.Color, sprite.Texture);
			
	}

	Entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

}