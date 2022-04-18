#pragma once

#include "Cober/Core.h"
#include "Cober/Timestep.h"
#include "Cober/Events/EventManager.h"

namespace Cober {
	
	class Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
		 
	protected:
		std::string m_DebugName;
	};
}

