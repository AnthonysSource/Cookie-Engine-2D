#pragma once

#include "Core/Window.h"

namespace Cookie {

	namespace Platform {

		void Initialize(WindowDescription *wd);
		bool IsRunning(GLFWwindow *appData);
		void Shutdown();

	} // namespace Platform

} // namespace Cookie