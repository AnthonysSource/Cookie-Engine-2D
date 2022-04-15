#pragma once

namespace Cookie {

// Asserts
#ifdef COOKIE_ASSERTS
#include <cstdlib>
#define COOKIE_ASSERT(x, msg)                                                                      \
	do {                                                                                           \
		if (!(x)) {                                                                                \
			Cookie::Logging::info("msg");                                                          \
			abort();                                                                               \
		}                                                                                          \
	} while (0)
#else
#define COOKIE_ASSERT(x, msg)
#endif

// MSVC Force inline
#define CKE_FORCE_INLINE __forceinline

} // namespace Cookie