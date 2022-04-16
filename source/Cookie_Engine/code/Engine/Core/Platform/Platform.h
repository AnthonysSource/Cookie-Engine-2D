#pragma once

#include "Core/Application.h"

namespace Cookie {
	namespace Platform {
		void Init();
		bool IsRunning(GLFWwindow *appData);
		void Shutdown();
	} // namespace Platform
} // namespace Cookie