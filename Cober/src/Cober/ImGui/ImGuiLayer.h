#pragma once

#include "Cober/Layers/Layer.h"
#include "Cober/Application.h"

namespace Cober {

	class CB_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

<<<<<<< Updated upstream
		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(SDL_Event* event);
=======
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(SDL_Event& event) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
>>>>>>> Stashed changes
	private:
		float m_Time = 0.0f;
		Application app = Application::Get();
	};
}


