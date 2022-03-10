#pragma once

#include "Cober/Layers/Layer.h"

namespace Cober {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(SDL_Event& event) override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}


