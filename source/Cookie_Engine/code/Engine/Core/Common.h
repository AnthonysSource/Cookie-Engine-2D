#pragma once

#include <Ckpch.h>
#include <cstdint>

namespace Cookie {

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using c16 = char16_t;
using c32 = char32_t;

using String = std::string;

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T> using WeakRef = std::weak_ptr<T>;
template <typename T> using Func = std::function<T>;

} // namespace Cookie

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