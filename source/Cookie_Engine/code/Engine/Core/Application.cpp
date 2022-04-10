#include "Application.h"

#include "Core/Common.h"
#include "Core/Logging/Logging.h"
#include "Core/Platform/Platform.h"
#include "Core/Platform/Window.h"

#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileSystemTest.h"

#include "Core/InputSystem/InputSystem.h"
#include "Core/RenderingSystem/RenderingSystem.h"

#include "EntityObjectModel/EntityAdmin.h"

#include <GLFW/glfw3.h>

namespace Cookie {

EntityAdmin *g_Admin = new EntityAdmin();

namespace Application {

	Window window;
	RenderingSystem *g_RenderingSystem = new RenderingSystem();

	void Run() {
		CKE_LOG_INFO("Starting up Cookie Engine");
		Platform::Init();

		CKE_LOG_INFO("Creating window");
		WindowManagement::CreateAppWindow(&window, 1280, 720, "Cookie Engine");

		CKE_LOG_INFO("Initializing Input System");
		InputSystem::Init(&window);

		g_Admin->Init();
		g_Admin->RegisterSystem(g_RenderingSystem);

		CKE_LOG_INFO("Initializing Rendering System");
		g_RenderingSystem->Init();

		CKE_LOG_INFO("Starting engine loop");
		while (Platform::IsRunning(window.m_Window)) {
			InputSystem::Update();
			g_RenderingSystem->Update(0.001f);
		}

		// Shutdown
		CKE_LOG_INFO("Shutting down");
		g_RenderingSystem->Shutdown();
		InputSystem::Shutdown();
		Platform::Shutdown();

		delete g_Admin;
		delete g_RenderingSystem;
	}

} // namespace Application
} // namespace Cookie