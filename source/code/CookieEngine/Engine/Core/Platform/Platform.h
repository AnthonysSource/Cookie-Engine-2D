#pragma once

#include "Core/Window.h"

namespace Cookie {

	namespace Platform {

		void Initialize(WindowDescription *wd);
		bool IsRunning(void *windowHandle);
		void Shutdown();

	} // namespace Platform

} // namespace Cookie