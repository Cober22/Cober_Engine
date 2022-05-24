#pragma once

#include <Engine.h>
#include "Cober/Scene/SceneSerializer.h"

using namespace Cober;
using namespace glm;

class Sandbox2D : public Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(Event& event) override;
private:
	Ref<Scene> m_ActiveScene;
};
