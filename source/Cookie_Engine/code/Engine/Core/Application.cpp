#include "Application.h"
#include "BasicTypes.h"
#include "InputSystem/InputSystem.h"
#include "Logging/Logging.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "FileSystem/FileSystem.h"
#include "FileSystem/FileSystemTest.h"

#include "Core/RenderingSystem/RenderingSystem.h"

namespace Cookie {
namespace Application {

	AppData appData;

	void Application::Init() {
		FileSystem::RunFileSystemTest();

		COOKIE_LOG_INFO("Starting up Cookie Engine");
		// Init and create window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		COOKIE_LOG_INFO("Creating window");
		appData.m_Window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);
		if (!appData.m_Window) {
			glfwTerminate();
		}

		COOKIE_LOG_INFO("Setting up graphics context and OpenGL");
		// Set current OpenGL context to the window
		glfwMakeContextCurrent(appData.m_Window);

		// Setup OpenGL function loading
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		COOKIE_LOG_INFO("Initializing input system");
		InputSystem::Init(appData.m_Window);
		RenderingSystem::Init();

		COOKIE_LOG_INFO("Starting engine loop");
		while (!glfwWindowShouldClose(appData.m_Window)) {
			RenderingSystem::Render();
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