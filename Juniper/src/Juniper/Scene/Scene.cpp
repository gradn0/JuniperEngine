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

		// Update animations
		auto& animationView = GetView<AnimationComponent>();
		for (auto [entity, animation] : animationView.each())
		{
			auto& frames = animation.Frames;
			auto& frameTime = animation.FrameTime;
			auto& currentFrame = animation.CurrentFrame;
			auto& elapsedTime = animation.ElapsedTime;

			if (frames.empty()) return;

			elapsedTime += dt;
			while (elapsedTime >= frameTime)
			{
				elapsedTime -= frameTime;
				currentFrame++;

				if (currentFrame >= frames.size())
					currentFrame = animation.Loop ? 0 : static_cast<uint32_t>(frames.size() - 1);
			}
		}

		// Render sprites
		auto& spriteView = GetView<SpriteComponent, TransformComponent>();
		for (auto [entity, sprite, transform] : spriteView.each())
		{
			auto& texture = sprite.Texture;

			// Use texture from the animation component if present
			if (HasComponents<AnimationComponent>(entity))
			{
				auto& animation = Get<AnimationComponent>(entity);
				texture = animation.Frames.at(animation.CurrentFrame);
			}

			Renderer::SubmitQuad(transform.GetTransform(), sprite.Color, texture);
		}
			
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