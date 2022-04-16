#include "Application.h"

#include "Core/Logging/Log.h"
#include "Core/Platform/Platform.h"
#include "Core/Platform/Window.h"
#include "Core/Types/Function.h"
#include "Core/Types/PrimitiveTypes.h"

#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileSystemTest.h"

#include "Core/InputSystem/InputSystem.h"
#include "Render/RenderingSystem.h"

#include "Entities/EntityAdmin.h"

#include <GLFW/glfw3.h>

namespace Cookie {

namespace Application {

	Window g_Window;
	EntityAdmin *g_Admin = new EntityAdmin();
	RenderingSystem *g_RenderingSystem = new RenderingSystem();

	void Run(TFunction<void()> CreateWorld) {
		CKE_LOG_INFO("Starting up Cookie Engine");
		Platform::Init();

		CKE_LOG_INFO("Creating window");
		WindowManagement::CreateAppWindow(&g_Window, 1280, 720, "Cookie Engine");

		CKE_LOG_INFO("Initializing Input System");
		InputSystem::Init(&g_Window);

		CKE_LOG_INFO("Initializing Entity Admin");
		g_Admin->Init();
		g_Admin->RegisterComponent<TransformComponent>();
		g_Admin->RegisterComponent<RenderComponent>();
		g_Admin->RegisterSystem(g_RenderingSystem);

		CreateWorld();

		CKE_LOG_INFO("Initializing Rendering System");
		g_RenderingSystem->Init();

		CKE_LOG_INFO("Starting engine loop");
		f32 tLastFrame = glfwGetTime();
		f32 tCurrentFrame = tLastFrame;

		while (Platform::IsRunning(g_Window.m_Window)) {
			tCurrentFrame = glfwGetTime();
			f64 deltaTime = tCurrentFrame - tLastFrame;
			tLastFrame = tCurrentFrame;

			InputSystem::Update();
			g_RenderingSystem->Update(deltaTime);
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