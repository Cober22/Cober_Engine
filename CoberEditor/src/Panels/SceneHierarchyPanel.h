#pragma once

#include "Cober/Core/Core.h"
#include "Cober/Core/Log.h"
#include "Cober/Scene/Scene.h"
#include "Cober/Scene/Entity.h"

#include <filesystem>

namespace Cober {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel();
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);

		template<typename T>
		void AddIfHasComponent(std::string name);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}
