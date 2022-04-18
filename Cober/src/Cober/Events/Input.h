#pragma once

#include "Cober/Core.h"
#include "Codes/KeyCodes.h"
#include "Codes/MouseCodes.h"

namespace Cober {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}