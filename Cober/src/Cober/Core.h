#pragma once

#ifdef CB_PLATFORM_WINDOWS
	#define CB_API
#else
	#error CoberEngine only supports Windows!
#endif


#ifdef CB_DEBUG
	#define CB_ENABLE_ASSERTS
#endif

#ifdef CB_ENABLE_ASSERTS
	#define CB_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CB_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CB_ASSERT(x, ...)
	#define CB_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)


