#pragma once

#include <bitset>
#include <queue>
#include <set>
#include <vector>

namespace Cookie {

	template <typename T> using TVector = std::vector<T>;
	template <typename T> using TSet = std::set<T>;
	template <typename T> using TQueue = std::queue<T>;

	template <size_t T> using Bitset = std::bitset<T>;

} // namespace Cookie