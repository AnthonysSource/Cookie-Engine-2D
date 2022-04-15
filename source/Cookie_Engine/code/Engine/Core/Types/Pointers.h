#pragma once

namespace Cookie {

	template <typename T> using Ref = std::shared_ptr<T>;
	template <typename T> using WeakRef = std::weak_ptr<T>;

    // This isn't really a pointer but until we find a better
    // place to put it we can define it here
	template <typename T> using Func = std::function<T>;
    
} // namespace Cookie