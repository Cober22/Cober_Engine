#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define HZ_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#
"x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define HZ_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define HZ_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


// DLL support
#ifdef HZ_PLATFORM_WINDOWS
	#if HZ_DYNAMIC_LINK
		#ifdef HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)
		#else
			#define HAZEL_API __declspec(dllimport)
		#endif
	#else
		#define HAZEL_API
	#endif
#else
	#error Hazel only supports Windows!
#endif // End of DLL support



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



// Common
#define GLCheckError() (glGetError() == GL_NO_ERROR)

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0
#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1
#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2
#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3
#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4
#define MOTION_TEXTURE_UNIT             GL_TEXTURE5
#define MOTION_TEXTURE_UNIT_INDEX       5
#define SPECULAR_EXPONENT_UNIT             GL_TEXTURE6
#define SPECULAR_EXPONENT_UNIT_INDEX       6
#define CASCACDE_SHADOW_TEXTURE_UNIT0       SHADOW_TEXTURE_UNIT
#define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX SHADOW_TEXTURE_UNIT_INDEX
#define CASCACDE_SHADOW_TEXTURE_UNIT1       GL_TEXTURE6
#define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX 6
#define CASCACDE_SHADOW_TEXTURE_UNIT2       GL_TEXTURE7
#define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX 7

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

//#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |  aiProcess_JoinIdenticalVertices)
#define ASSIMP_LOAD_FLAGS (aiProcess_FlipUVs |	aiProcess_JoinIdenticalVertices | aiProcess_ValidateDataStructure |	aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_GenUVCoords | aiProcess_TransformUVCoords |	aiProcess_LimitBoneWeights | aiProcess_OptimizeMeshes |	aiProcess_GenSmoothNormals | aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_SortByPType)

			//aiProcess_FlipUVs |
			//aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
			//aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
			//aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
			//aiProcess_RemoveRedundantMaterials |	// remove redundant materials
			//aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
			//aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
			////aiProcess_FindInstances |				// search for instanced meshes and remove them by references to one master
			//aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
			//aiProcess_OptimizeMeshes |				// join small meshes, if possible;
			////aiProcess_PreTransformVertices |		
			//aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
			//aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
			//aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
			//aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
			//aiProcess_SortByPType);

namespace Cober {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}


