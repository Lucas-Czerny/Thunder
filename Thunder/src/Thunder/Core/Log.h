#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#include "Core.h"

namespace Thunder
{
	class THUNDER_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();

		inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

// Core log macros

#define TD_CORE_TRACE(...)  Thunder::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TD_CORE_INFO(...)   Thunder::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TD_CORE_WARN(...)   Thunder::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TD_CORE_ERROR(...)  Thunder::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TD_CORE_FATAL(...)  Thunder::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros

#define TD_TRACE(...)       Thunder::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TD_INFO(...)        Thunder::Log::GetClientLogger()->info(__VA_ARGS__)
#define TD_WARN(...)        Thunder::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TD_ERROR(...)       Thunder::Log::GetClientLogger()->error(__VA_ARGS__)
#define TD_FATAL(...)       Thunder::Log::GetClientLogger()->fatal(__VA_ARGS__)