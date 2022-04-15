#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Cookie {
namespace Log {
	
	namespace Internal {
		Ref<spdlog::logger> g_pEngineLogger = spdlog::stdout_color_mt("Engine");
	}

} // namespace Log
} // namespace Cookie