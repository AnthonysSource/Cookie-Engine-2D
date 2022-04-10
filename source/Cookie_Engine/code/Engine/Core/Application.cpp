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

class CustomSystem : public System {
  public:
	void Init() { m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true); }

	void Update(f32 dt) override {
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			t->m_Rotation += vec3(0.0f, 5.0f, 0.0f) * dt;
		}
	}
};

namespace Application {

	Window window;
	RenderingSystem *g_RenderingSystem = new RenderingSystem();
	CustomSystem *g_CustomSystem = new CustomSystem();

	void Run() {
		CKE_LOG_INFO("Starting up Cookie Engine");
		Platform::Init();

		CKE_LOG_INFO("Creating window");
		WindowManagement::CreateAppWindow(&window, 1280, 720, "Cookie Engine");

		CKE_LOG_INFO("Initializing Input System");
		InputSystem::Init(&window);

		CKE_LOG_INFO("Initializing Entity Admin");
		g_Admin->Init();
		g_Admin->RegisterSystem(g_RenderingSystem);
		g_Admin->RegisterSystem(g_CustomSystem);

		CKE_LOG_INFO("Initializing Rendering System");
		g_RenderingSystem->Init();

		CKE_LOG_INFO("Initializing Custom System");
		g_CustomSystem->Init();

		CKE_LOG_INFO("Starting engine loop");
		while (Platform::IsRunning(window.m_Window)) {
			InputSystem::Update();
			f32 deltaTime = 0.001f;
			g_RenderingSystem->Update(deltaTime);
			g_CustomSystem->Update(deltaTime);
		}

		// Shutdown
		CKE_LOG_INFO("Shutting down");
		g_RenderingSystem->Shutdown();
		InputSystem::Shutdown();
		Platform::Shutdown();

		delete g_Admin;
		delete g_RenderingSystem;
		delete g_CustomSystem;
	}

} // namespace Application
} // namespace Cookie