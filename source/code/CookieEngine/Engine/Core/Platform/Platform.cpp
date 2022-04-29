#include "Platform.h"

#include <GLFW/glfw3.h>

namespace Cookie::Platform {

	void Init(WindowDescription *wd) {
		WindowData *window = &g_AppData.m_Window;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (wd->m_RefreshRate != 0) {
			glfwWindowHint(GLFW_REFRESH_RATE, wd->m_RefreshRate);
		} else {
			glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
		}

		if (wd->m_IsFullScreen) {
			window->m_Handle = glfwCreateWindow(wd->m_Width, wd->m_Height, wd->m_Title.c_str(), glfwGetPrimaryMonitor(), NULL);
		} else {
			window->m_Handle = glfwCreateWindow(wd->m_Width, wd->m_Height, wd->m_Title.c_str(), NULL, NULL);
		}

		if (!window->m_Handle) {
			glfwTerminate();
		}

		window->m_Width = wd->m_Width;
		window->m_Height = wd->m_Height;
		window->m_Title = wd->m_Title;
	}

	bool IsRunning(GLFWwindow *window) { return !glfwWindowShouldClose(window); }

	void Shutdown() { glfwTerminate(); }

} // namespace Cookie::Platform