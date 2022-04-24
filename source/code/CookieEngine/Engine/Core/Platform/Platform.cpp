#include "Platform.h"

#include <GLFW/glfw3.h>

namespace Cookie::Platform {

	void CreateAppWindow(WindowData *window, u32 width, u32 height, const char *title) {
		window->m_Handle = glfwCreateWindow(1280, 720, title, NULL, NULL);
		window->m_Width = 1280;
		window->m_Height = 720;
		window->m_Title = title;
		if (!window->m_Handle) {
			glfwTerminate();
		}
	};

	void Init(WindowDescription *wd) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		CreateAppWindow(&g_AppData.m_Window, wd->m_Width, wd->m_Height, wd->m_Title.c_str());
	}

	void CreateAppWindow(WindowData *window, u32 width, u32 height, const char *title);

	bool IsRunning(GLFWwindow *window) { return !glfwWindowShouldClose(window); }

	void Shutdown() { glfwTerminate(); }

} // namespace Cookie::Platform