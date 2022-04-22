#include "Application.h"

#include "Core/Logging/Log.h"
#include "Core/Platform/Platform.h"
#include "Core/Platform/Window.h"
#include "Core/Types/Function.h"
#include "Core/Types/PrimitiveTypes.h"

#include "Core/FileSystem/FileSystem.h"
#include "Core/FileSystem/FileSystemTest.h"

#include "Resources/Resources.h"

#include "Core/InputSystem/InputSystem.h"
#include "Render/RenderingSystem.h"

#include "Entities/EntityAdmin.h"

#include <GLFW/glfw3.h>

namespace Cookie {

	AppData g_AppData;
	EntityAdmin *g_Admin = new EntityAdmin();
	TimeData g_TimeData;
	RenderingSystem *g_RenderingSystem = new RenderingSystem();

	namespace Application {

		void Run(TFunction<void(EntityAdmin *const EntitiesAdmin)> CreateWorld) {

			Log::AddEntry(Log::Verbosity::Error, "Testing", "Random.cpp", 23, "Formatted Log Entry [%s]", "String2");

			CKE_LOG_INFO("Starting up Cookie Engine");
			Platform::Init();

			CKE_LOG_INFO("Creating window");
			WindowManagement::CreateAppWindow(&g_AppData.m_Window, 1280, 720, "Cookie Engine");

			CKE_LOG_INFO("Initializing Input System");
			InputSystem::Init(&g_AppData.m_Window);

			CKE_LOG_INFO("Initializing Resources System");
			ResourcesSystem::Init();

			CKE_LOG_INFO("Initializing Rendering System");
			g_RenderingSystem->Init();

			CKE_LOG_INFO("Initializing Entity Admin");
			g_Admin->Init();
			g_Admin->RegisterComponent<TransformComponent>();
			g_Admin->RegisterComponent<RenderComponent>();
			g_Admin->RegisterSystem(g_RenderingSystem);

			CreateWorld(g_Admin);

			CKE_LOG_INFO("Starting engine loop");

			g_TimeData.m_SecondsUpTimeLastUpdate = glfwGetTime();
			g_TimeData.m_SecondsUpTime = g_TimeData.m_SecondsUpTimeLastUpdate;

			while (Platform::IsRunning(g_AppData.m_Window.m_Handle)) {
				g_TimeData.m_SecondsUpTime = glfwGetTime();
				g_TimeData.m_DeltaTime = g_TimeData.m_SecondsUpTime - g_TimeData.m_SecondsUpTimeLastUpdate;
				g_TimeData.m_SecondsUpTimeLastUpdate = g_TimeData.m_SecondsUpTime;

				InputSystem::Update();
				g_Admin->Update(g_TimeData.m_DeltaTime);
				g_RenderingSystem->Update(g_TimeData.m_DeltaTime);
			}

			// Shutdown
			CKE_LOG_INFO("Shutting down");
			g_RenderingSystem->Shutdown();
			InputSystem::Shutdown();
			Platform::Shutdown();
			ResourcesSystem::Shutdown();

			delete g_Admin;
			delete g_RenderingSystem;
		}

	} // namespace Application
} // namespace Cookie