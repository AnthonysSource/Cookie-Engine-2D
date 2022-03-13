#pragma once

namespace Cookie {
namespace Log {

void Init();
void Shutdown();

void Info(const char *format, ...);
void Warning(const char *format, ...);
void Error(const char *format, ...);
void Assert(const char *format, ...);

} // namespace Logging
} // namespace Cookie

#ifdef COOKIE_LOGGING
#define COOKIE_LOG_INFO(...)                                                   \
	{ Cookie::Log::Trace(__VA_ARGS__); }
#define COOKIE_LOG_WARNING(...)                                                \
	{ Cookie::Log::Warning(__VA_ARGS__); }
#define COOKIE_LOG_ERROR(...)                                                  \
	{ Cookie::Log::Error(__VA_ARGS__); }
#define COOKIE_LOG_ASSERT(...)                                                 \
	{ Cookie::Log::Assert(__VA_ARGS__); }
#else
#define COOKIE_LOG_INFO(...)
#define COOKIE_LOG_WARNING(...)
#define COOKIE_LOG_ERROR(...)
#define COOKIE_LOG_ASSERT(...)
#endif