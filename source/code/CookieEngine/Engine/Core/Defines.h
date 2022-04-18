#pragma once

#include "Core/Logging/Log.h"

namespace Cookie {

// Asserts
#ifdef CKE_ASSERTS
	#include <cstdlib>
	#define CKE_ASSERT(x, msg)                                                                                                             \
		do {                                                                                                                               \
			if (!(x)) {                                                                                                                    \
				CKE_LOG_INFO(msg);                                                                                                         \
				abort();                                                                                                                   \
			}                                                                                                                              \
		} while (0)
#else
	#define CKE_ASSERT(x, msg)
#endif

// MSVC Force inline
#define CKE_FORCE_INLINE __forceinline

} // namespace Cookie