#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Cober/Log.h"
#include "Cober/Instrumentator.h"

#define GLFW_INCLUDE_NONE
#include <Glew/glew.h>
#include <GLFW/glfw3.h>
//#include <SDL/SDL.h>


#ifdef CB_PLATFORM_WINDOWS
	#include <Windows.h>
#endif