#pragma once

#include <GLFW/glfw3.h>

namespace Cookie {
namespace InputSystem {

	void Init(GLFWwindow *window);
	void Update();
	void Shutdown();

} // namespace InputSystem
} // namespace Cookie