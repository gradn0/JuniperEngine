#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Juniper/Renderer/Renderer.h"

namespace Juniper {

	void Scene::OnUpdate(float dt)
	{
		auto& view = GetView<SpriteComponent, TransformComponent>();
		for (auto [entity, sprite, transform] : view.each())
			Renderer::SubmitQuad(transform.Translation, sprite.Size, sprite.Color, sprite.Texture);
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