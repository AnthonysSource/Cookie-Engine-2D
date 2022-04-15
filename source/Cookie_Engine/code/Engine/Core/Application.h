#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/String.h"

#include "GLFW/glfw3.h"

namespace Cookie {

struct Window {
	GLFWwindow *m_Window;
	u32 m_Height;
	u32 m_Width;
	String m_Title;
};

namespace Application {

	extern Window window;

	void Run();

} // namespace Application
} // namespace Cookie