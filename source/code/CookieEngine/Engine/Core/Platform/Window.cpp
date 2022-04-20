#include "Window.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Cookie {

	namespace WindowManagement {

		void CreateAppWindow(WindowData *window, u32 width, u32 height, const char *title) {
			window->m_Handle = glfwCreateWindow(1280, 720, title, NULL, NULL);
			window->m_Width = 1280;
			window->m_Height = 720;
			window->m_Title = title;
			if (!window->m_Handle) {
				glfwTerminate();
			}
		};

		void SetWindowSize(WindowData *w, u32 width, u32 height) { glfwSetWindowSize(w->m_Handle, width, height); }

	} // namespace WindowManagement
} // namespace Cookie