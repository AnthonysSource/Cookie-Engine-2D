#include "Application.h"

#include "Core/Common.h"
#include "Core/Logging/Logging.h"
#include "Core/Platform/Platform.h"
#include "Core/Platform/Window.h"

#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileSystemTest.h"
#include "Core/InputSystem/InputSystem.h"
#include "Core/RenderingSystem/RenderingSystem.h"

#include <GLFW/glfw3.h>

namespace Cookie {
namespace Application {

	Window window;

	void Run() {
		CKE_LOG_INFO("Starting up Cookie Engine");
		Platform::Init();

		CKE_LOG_INFO("Creating window");
		WindowManagement::CreateAppWindow(&window, 1280, 720, "Cookie Engine");

		CKE_LOG_INFO("Initializing Input System");
		InputSystem::Init(&window);

		CKE_LOG_INFO("Initializing Rendering System");
		RenderingSystem::Init();

		CKE_LOG_INFO("Starting engine loop");
		while (Platform::IsRunning(window.m_Window)) {
			InputSystem::Update();
			RenderingSystem::Render();
		}

		// Shutdown
		CKE_LOG_INFO("Shutting down");
		RenderingSystem::Shutdown();
		InputSystem::Shutdown();
		Platform::Shutdown();
	}

} // namespace Application
} // namespace Cookie