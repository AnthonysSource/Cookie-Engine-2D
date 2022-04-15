#pragma once

#include "Core/Common.h"

#include <spdlog/spdlog.h>

namespace Cookie {
namespace Log {

	void Initialize();
	void Shutdown();

	void AddLine(int const channel, ...);

	namespace Internal {
		extern Ref<spdlog::logger> g_pEngineLogger;
	}

} // namespace Log

} // namespace Cookie

#ifdef COOKIE_LOGGING
#define CKE_LOG_INFO(...)                                                                          \
	{ Cookie::Log::Internal::g_pEngineLogger->info(__VA_ARGS__); }
#else
#define CKE_LOG_INFO(...)
#define CKE_LOG_WARNING(...)
#define CKE_LOG_ERROR(...)
#define CKE_LOG_ASSERT(...)
#endif