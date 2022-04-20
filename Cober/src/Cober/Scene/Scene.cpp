#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Cober/Renderer/Renderer.h"

#include <glm/glm.hpp> 

namespace Cober {

	static void DoMath(const glm::mat4& transform) {

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		
		// Get and Entity with the given Component
		if (m_Registry.any_of<TransformComponent>(entity))	//	m_Registry.all_of<TransformComponent>(entity)
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		
		// Iterate through entities with TransformComponent
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) 
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
#endif
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity() {

		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts) {

		// Iterate through entities with TransformComponent
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {

			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer::DrawQuad(transform, sprite.Color);
		}
	}

}