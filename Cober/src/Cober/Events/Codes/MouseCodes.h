#pragma once

#include <ostream>

namespace Cober
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define MOUSE_BUTTON_0      ::Cober::Mouse::Button0
#define MOUSE_BUTTON_1      ::Cober::Mouse::Button1
#define MOUSE_BUTTON_2      ::Cober::Mouse::Button2
#define MOUSE_BUTTON_3      ::Cober::Mouse::Button3
#define MOUSE_BUTTON_4      ::Cober::Mouse::Button4
#define MOUSE_BUTTON_5      ::Cober::Mouse::Button5
#define MOUSE_BUTTON_6      ::Cober::Mouse::Button6
#define MOUSE_BUTTON_7      ::Cober::Mouse::Button7
#define MOUSE_BUTTON_LAST   ::Cober::Mouse::ButtonLast
#define MOUSE_BUTTON_LEFT   ::Cober::Mouse::ButtonLeft
#define MOUSE_BUTTON_RIGHT  ::Cober::Mouse::ButtonRight
#define MOUSE_BUTTON_MIDDLE ::Cober::Mouse::ButtonMiddle

