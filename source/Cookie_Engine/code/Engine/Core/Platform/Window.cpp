#include "Window.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Cookie {

namespace WindowManagement {

	void CreateAppWindow(Window *window, u32 width, u32 height, const char *title) {
		window->m_Window = glfwCreateWindow(1280, 720, title, NULL, NULL);
		window->m_Width = 1280;
		window->m_Height = 720;
		window->m_Title = title;
		if (!window->m_Window) {
			glfwTerminate();
		}
	};

	void SetWindowSize(Window *w, u32 width, u32 height) {
		glfwSetWindowSize(w->m_Window, width, height);
	}

} // namespace WindowManagement
} // namespace Cookie