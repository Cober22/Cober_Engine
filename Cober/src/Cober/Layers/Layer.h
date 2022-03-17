#pragma once

#include "Cober/Core.h"

#include "Cober/Timestep.h"

#include <SDL/SDL.h>

namespace Cober {
	
	class Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(SDL_Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
		 
	protected:
		std::string m_DebugName;
	};
}

