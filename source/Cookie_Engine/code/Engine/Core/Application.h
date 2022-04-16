#pragma once

#include "Core/Types/Function.h"
#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/String.h"

#include "EntityObjectModel/EntityAdmin.h"

#include "GLFW/glfw3.h"

namespace Cookie {

struct Window {
	GLFWwindow *m_Window;
	u32 m_Height;
	u32 m_Width;
	String m_Title;
};

namespace Application {

	extern Window g_Window;
	extern EntityAdmin *g_Admin;

	void Run(TFunction<void()> CreateWorld);

} // namespace Application
} // namespace Cookie