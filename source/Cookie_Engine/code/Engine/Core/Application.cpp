#include "Application.h"

#include "Core/Common.h"
#include "Core/Logging/Logging.h"
#include "Core/Platform/Platform.h"

#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileSystemTest.h"
#include "Core/InputSystem/InputSystem.h"
#include "Core/RenderingSystem/RenderingSystem.h"

#include <GLFW/glfw3.h>

namespace Cookie {
namespace Application {

	AppData appData;

	void Application::Init() {
		COOKIE_LOG_INFO("Starting up Cookie Engine");
		Platform::Init();

		COOKIE_LOG_INFO("Creating window");
		appData.m_Window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);
		if (!appData.m_Window) {
			glfwTerminate();
		}

		COOKIE_LOG_INFO("Initializing Input System");
		InputSystem::Init(appData.m_Window);

		COOKIE_LOG_INFO("Initializing Rendering System");
		RenderingSystem::Init();

		COOKIE_LOG_INFO("Starting engine loop");
		while (Platform::IsRunning(appData.m_Window)) {
			RenderingSystem::Render();
			glfwPollEvents();
			InputSystem::Update();
		}
	}

	void Application::Shutdown() {
		COOKIE_LOG_INFO("Shutting down");
		RenderingSystem::Shutdown();
		InputSystem::Shutdown();
		Platform::Shutdown();
	}

} // namespace Application
} // namespace Cookie