#include "pch.h"
#include "Scene.h"

namespace Juniper {

	Entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

}