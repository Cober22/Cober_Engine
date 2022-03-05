#pragma once

#include "Cober/Layers/Layer.h"
#include "Cober/Application.h"

namespace Cober {

	class CB_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(SDL_Event& event);
	private:
		float m_Time = 0.0f;
		Application app = Application::Get();
	};
}


