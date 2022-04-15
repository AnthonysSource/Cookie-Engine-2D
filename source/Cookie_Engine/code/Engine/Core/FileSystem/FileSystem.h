#pragma once

#include "Core/Types/String.h"

namespace Cookie {
namespace FileSystem {

	void WriteToFile(const char *path, const char *data, const size_t size);
	void ReadFile(const char *path, char *outputBuffer, const size_t size);

	String ReadTextFile(const char *path);

} // namespace FileSystem
} // namespace Cookie