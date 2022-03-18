#pragma once
#include "Ckpch.h"

namespace Cookie {
namespace FileSystem {

void WriteToFile(const char *path, const char *data, const size_t size);
void ReadFile(const char *path, char *outputBuffer, const size_t size);

} // namespace FileSystem
} // namespace Cookie