#pragma once
#include "GLFW/glfw3.h"

namespace Cookie {

struct AppData {
	GLFWwindow *m_Window;
};

namespace Application {

	extern AppData appData;

	void Init();
	void Shutdown();

} // namespace Application
} // namespace Cookie