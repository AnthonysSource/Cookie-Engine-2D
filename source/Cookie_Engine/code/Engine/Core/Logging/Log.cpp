#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Cookie {
	namespace Log {

		namespace Internal {
			TSharedPtr<spdlog::logger> g_pEngineLogger = spdlog::stdout_color_mt("Engine");
		}

	} // namespace Log
} // namespace Cookie