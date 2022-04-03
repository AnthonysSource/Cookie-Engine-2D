#include "Platform.h"

#include <GLFW/glfw3.h>

namespace Cookie {
namespace Platform {
	void Init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	bool IsRunning(GLFWwindow* window) { return !glfwWindowShouldClose(window); }

	void Shutdown() { glfwTerminate(); }
} // namespace Platform
} // namespace Cookie