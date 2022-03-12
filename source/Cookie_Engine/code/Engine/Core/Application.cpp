#include "Application.h"
#include "Logging.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cookie {
namespace Application {

// Ptr to the application window
GLFWwindow *s_window;

void Application::init() {
	// Init and create window
	glfwInit();
	s_window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);
	if (!s_window) {
		glfwTerminate();
	}

	// Set current OpenGL context to the window
	glfwMakeContextCurrent(s_window);

	// Setup OpenGL function loading
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Logging::info("Logging Test");

	// Engine Loop
	while (!glfwWindowShouldClose(s_window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(s_window);
		glfwPollEvents();
	}
}

void Application::shutdown() { glfwTerminate(); }

} // namespace Application
} // namespace Cookie