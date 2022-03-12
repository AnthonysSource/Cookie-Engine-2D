#pragma once

namespace Cookie {
namespace Logging {

void init();
void shutdown();

void info(const char *format, ...);
void warning(const char *format, ...);
void error(const char *format, ...);
void assert(const char *format, ...);

} // namespace Logging
} // namespace Cookie

#ifdef COOKIE_LOGGING
#define COOKIE_LOG_INFO(...)                                                   \
	{ Cookie::Logging::trace(__VA_ARGS__); }
#define COOKIE_LOG_WARNING(...)                                                \
	{ Cookie::Logging::warning(__VA_ARGS__); }
#define COOKIE_LOG_ERROR(...)                                                  \
	{ Cookie::Logging::error(__VA_ARGS__); }
#define COOKIE_LOG_ASSERT(...)                                                 \
	{ Cookie::Logging::assert(__VA_ARGS__); }
#else
#define COOKIE_LOG_INFO(...)
#define COOKIE_LOG_WARNING(...)
#define COOKIE_LOG_ERROR(...)
#define COOKIE_LOG_ASSERT(...)
#endif