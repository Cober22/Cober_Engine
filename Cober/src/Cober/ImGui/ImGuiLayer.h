#pragma once

#include "Cober/Layers/Layer.h"
#include "Cober/Events/ApplicationEvents.h"
#include "Cober/Events/KeyEvents.h"
#include "Cober/Events/MouseEvents.h"

#include "ImGui/imgui.h"

namespace Cober {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
		GLFWwindow* window;
	};
}


