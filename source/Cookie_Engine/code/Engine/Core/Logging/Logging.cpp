#include "Logging.h"
#include "Ckpch.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Cookie {
namespace Log {

	namespace Internal {
		std::shared_ptr<spdlog::logger> engineLogger = spdlog::stdout_color_mt("Engine");
	}
    
} // namespace Log
} // namespace Cookie