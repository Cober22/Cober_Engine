#pragma once

#include <memory>

#ifdef CB_PLATFORM_WINDOWS
	#define CB_API
#else
	#error CoberEngine only supports Windows!
#endif

#ifdef CB_DEBUG
	#define CB_ENABLE_ASSERTS
#endif

#ifdef CB_ENABLE_ASSERTS
	#define CB_ASSERT(x, ...) { if(!(x)) { CB_LogError(1, __VA_ARGS__); __debugbreak(); } }
#else
	#define CB_ASSERT(x, ...)
#endif

//#define BIT(x) (1 << x) 
// 
//#define CB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Cober {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}


