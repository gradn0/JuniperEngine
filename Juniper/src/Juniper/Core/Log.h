#pragma once

#include "pch.h"
#include "spdlog/spdlog.h"
#include "Core.h"

namespace Juniper {

	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core macros
#define JP_CORE_ERROR(...) ::Juniper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define JP_CORE_WARN(...) ::Juniper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define JP_CORE_INFO(...) ::Juniper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define JP_CORE_TRACE(...) ::Juniper::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define JP_CORE_CRITICAL(...) ::Juniper::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client macros
#define JP_CLIENT_ERROR(...) ::Juniper::Log::GetClientLogger()->error(__VA_ARGS__)
#define JP_CLIENT_WARN(...) ::Juniper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define JP_CLIENT_INFO(...) ::Juniper::Log::GetClientLogger()->info(__VA_ARGS__)
#define JP_CLIENT_TRACE(...) ::Juniper::Log::GetClientLogger()->trace(__VA_ARGS__)
#define JP_CLIENT_CRITICAL(...) ::Juniper::Log::GetClientLogger()->critical(__VA_ARGS__)

// Asserts
#ifdef JP_ASSERTS
	#define JP_CORE_ASSERT(x, ...) { if (!(x)) { JP_CLIENT_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define JP_CLIENT_ASSERT(x, ...) { if (!(x)) { JP_CLIENT_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define JP_CORE_VERIFY(x, ...) { if (!(x)) { JP_CLIENT_ERROR("Verification failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define JP_CLIENT_VERIFY(x, ...) { if (!(x)) { JP_CLIENT_ERROR("Verification failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define JP_CORE_ASSERT(x, ...)
	#define JP_CLIENT_ASSERT(x, ...)
	#define JP_CORE_VERIFY(x, ...) (x)
	#define JP_CLIENT_VERIFY(x, ...) (x)
#endif