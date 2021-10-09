#pragma once

#include <memory>

#ifdef TD_PLATFORM_WINDOWS
	#ifdef TD_DYNAMIC_LINKING
		#ifdef TD_BUILD_DLL
			#define THUNDER_API __declspec(dllexport)
		#else
			#define THUNDER_API __declspec(dllimport)
		#endif
	#else
		#define THUNDER_API
	#endif
#else
	#error Thunder only supports Windows!
#endif

#if TD_ENABLE_ASSERTS
	#define TD_ASSERT(x, ...) { \
				if (!x) { TD_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TD_CORE_ASSERT(x, ...) { \
				if (!x) { TD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TD_ASSERT(x, ...)
	#define TD_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define TD_BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Thunder
{
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename ... Args>
	Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}