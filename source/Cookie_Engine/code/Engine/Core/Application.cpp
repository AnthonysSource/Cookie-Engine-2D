#include "Application.h"
#include "InputSystem/InputSystem.h"
#include "Logging/Logging.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "FileSystem/FileSystem.h"
#include "FileSystem/FileSystemTest.h"

namespace Cookie {
namespace Application {

// Ptr to the application window
GLFWwindow *s_Window;


void Application::Init() {

	FileSystem::RunFileSystemTest();

	COOKIE_LOG_INFO("Starting up Cookie Engine");
	// Init and create window
	glfwInit();

	COOKIE_LOG_INFO("Creating window");
	s_Window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);
	if (!s_Window) {
		glfwTerminate();
	}

	COOKIE_LOG_INFO("Setting up graphics context and OpenGL");
	// Set current OpenGL context to the window
	glfwMakeContextCurrent(s_Window);

	// Setup OpenGL function loading
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	COOKIE_LOG_INFO("Initializing input system");
	InputSystem::Init(s_Window);

	COOKIE_LOG_INFO("Starting engine loop");
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