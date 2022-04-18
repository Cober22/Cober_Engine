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
		virtual void OnEvent() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
		GLFWwindow* window;
	};
}


