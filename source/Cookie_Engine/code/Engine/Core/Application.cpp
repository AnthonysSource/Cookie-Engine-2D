#include "Application.h"
#include "InputSystem.h"
#include "Logging.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cookie {
namespace Application {

// Ptr to the application window
GLFWwindow *s_Window;

void Application::Init() {

	Log::Info("Starting up Cookie Engine");
	// Init and create window
	glfwInit();

	Log::Info("Creating window");
	s_Window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);
	if (!s_Window) {
		glfwTerminate();
	}

	Log::Info("Setting up graphics context and OpenGL");
	// Set current OpenGL context to the window
	glfwMakeContextCurrent(s_Window);

	// Setup OpenGL function loading
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Log::Info("Initializing input system");
	InputSystem::Init(s_Window);

	Log::Info("Starting engine loop");
	// Engine Loop
	while (!glfwWindowShouldClose(s_Window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(s_Window);
		glfwPollEvents();
		InputSystem::Update();
	}
}

void Application::Shutdown() {
	Log::Info("Shutting down");
	glfwTerminate();
}

} // namespace Application
} // namespace Cookie