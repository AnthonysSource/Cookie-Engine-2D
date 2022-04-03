#pragma once

#include <spdlog/spdlog.h>

namespace Cookie {
namespace Log {

	void Init();
	void Shutdown();

} // namespace Log
} // namespace Cookie

#ifdef COOKIE_LOGGING
#define CKE_LOG_INFO(...)                                                                       \
	{ spdlog::info(__VA_ARGS__); }
#else
#define CKE_LOG_INFO(...)
#define CKE_LOG_WARNING(...)
#define CKE_LOG_ERROR(...)
#define CKE_LOG_ASSERT(...)
#endif