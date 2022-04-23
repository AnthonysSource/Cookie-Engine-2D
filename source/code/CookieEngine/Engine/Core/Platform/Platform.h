#pragma once

#include "Core/Application.h"

namespace Cookie::Platform {

	void Init();
	bool IsRunning(GLFWwindow *appData);
	void Shutdown();

} // namespace Cookie::Platform