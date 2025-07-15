#pragma once

#include <entt.hpp>

namespace Juniper {

	using Entity = entt::entity;
	constexpr Entity NULL_ENTITY = entt::null;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		template<typename T, typename... Args>
		T& AddComponent(Entity entity, Args... args)
		{
			return m_Registry.emplace<T>(entity, args...);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			m_Registry.remove<T>(entity);
		}

		template<typename... Components>
		bool HasComponents(Entity entity)
		{
			return m_Registry.all_of<Components...>(entity);
		}

		template<typename T>
		T& Get(Entity entity)
		{
			return m_Registry.get<T>(entity);
		}

		template<typename... Components>
		auto GetView()
		{
			return m_Registry.view<Components...>();
		}

	private:
		entt::registry m_Registry;
	};

}