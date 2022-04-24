#pragma once

#include "Core/Application.h"

namespace Cookie::Platform {

	void Init(WindowDescription *wd);
	bool IsRunning(GLFWwindow *appData);
	void Shutdown();

} // namespace Cookie::Platform