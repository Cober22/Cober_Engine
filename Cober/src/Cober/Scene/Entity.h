#pragma once

#include "Scene.h"
//#include "Cober/Core/UUID.h"
//#include "Components.h"

#include "entt.hpp"

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
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
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
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }


		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }
		Scene* GetScene() { return m_Scene; }

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
		Entity GetEntity() { return m_Entity; }

		std::string ReadScript(const std::string& path) {
			std::string result;
			std::ifstream in(path, std::ios::in | std::ios::binary);
			if (in)
			{
				in.seekg(0, std::ios::end);
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else
				CB_LogError(LOG_APP, "Could not read the script");

			return result;
		}
	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};
		Entity Find(std::string name) {
			if (m_Entity) {
				std::list Entities = GetEntity().GetScene()->GetEntitiesOnScene();
				for (auto e : Entities)
					if (e.GetName() == name)
						return e;
			}
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}
