#pragma once

#include "Cober/Core.h"
#include "Cober/Log.h"
#include "Cober/Scene/Scene.h"
#include "Cober/Scene/Entity.h"

namespace Cober {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}