#pragma once

#include "Scene.h"
#include "ENTT/entt.hpp"

namespace Cober {

	class Entity {
	public:
		Entity() = default;
		Entity(const Entity& entity) = default;
		Entity::Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) { }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) { 
			CB_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...); 
		}

		template<typename T>
		T& GetComponent() { 
			CB_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle); 
		}

		template<typename T>
		bool HasComponent() { 
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle); 
		}

		template<typename T>
		void RemoveComponent() {
			CB_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const { return !(*this == other); }
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

	class ScriptableEntity {

	public:
		virtual ~ScriptableEntity() {};
		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>();	}

	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};
	private:
		Entity m_Entity;
		friend class Scene;
	};
}