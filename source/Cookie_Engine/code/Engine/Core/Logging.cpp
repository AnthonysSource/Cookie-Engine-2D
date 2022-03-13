#include "Logging.h"
#include "Engine/Ckpch.h"

namespace Cookie {
namespace Log {

void Log::Info(const char *format, ...) {
    // Use c++ formatting functions
	char msg[256];
	va_list ap;
	va_start(ap, format);
	vsnprintf(msg, sizeof(msg), format, ap);
	va_end(ap);

	std::cout << msg << std::endl;
}

} // namespace Logging
} // namespace Cookie