#pragma once

#include <spdlog/spdlog.h>

namespace Cookie {
namespace Log {

	void Init();
	void Shutdown();

} // namespace Log
} // namespace Cookie

#ifdef COOKIE_LOGGING
#define COOKIE_LOG_INFO(...)                                                                       \
	{ spdlog::info(__VA_ARGS__); }
#else
#define COOKIE_LOG_INFO(...)
#define COOKIE_LOG_WARNING(...)
#define COOKIE_LOG_ERROR(...)
#define COOKIE_LOG_ASSERT(...)
#endif