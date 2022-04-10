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

struct RotatingComponent {
	f32 m_Speed = 5.0f;
};

class RotateSystem : public System {
  public:
	void InitSignature() {
		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<RotatingComponent>(), true);
	}

	void Update(f32 dt) override {
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			RotatingComponent *f = g_Admin->GetComponent<RotatingComponent>(entityID);
			t->m_Rotation += vec3(0.0f, f->m_Speed, 0.0f) * dt;
		}
	}
};

namespace Application {

	Window window;
	RenderingSystem *g_RenderingSystem = new RenderingSystem();
	RotateSystem *g_CustomSystem = new RotateSystem();

	void Run() {
		CKE_LOG_INFO("Starting up Cookie Engine");
		Platform::Init();

		CKE_LOG_INFO("Creating window");
		WindowManagement::CreateAppWindow(&window, 1280, 720, "Cookie Engine");

		CKE_LOG_INFO("Initializing Input System");
		InputSystem::Init(&window);

		CKE_LOG_INFO("Initializing Entity Admin");
		g_Admin->Init();
		g_Admin->RegisterComponent<TransformComponent>();
		g_Admin->RegisterComponent<RotatingComponent>();
		g_Admin->RegisterSystem(g_RenderingSystem);
		g_Admin->RegisterSystem(g_CustomSystem);

		// Create Entities
		EntityID e = g_Admin->CreateEntity();
		TransformComponent t{};
		t.m_Position = vec3(-2.0f, 0.0f, 0.0f);
		g_Admin->AddComponent(e, t);

		e = g_Admin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = vec3(0.0f, 0.0f, 0.0f);
		RotatingComponent f{};
		f.m_Speed = 5.0f;
		g_Admin->AddComponent(e, t);
		g_Admin->AddComponent(e, f);

		e = g_Admin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = vec3(2.0f, 0.0f, 0.0f);
		f = RotatingComponent{};
		f.m_Speed = 15.0f;
		g_Admin->AddComponent(e, t);
		g_Admin->AddComponent(e, f);

		CKE_LOG_INFO("Initializing Rendering System");
		g_RenderingSystem->Init();

		CKE_LOG_INFO("Starting engine loop");
		while (Platform::IsRunning(window.m_Window)) {
			InputSystem::Update();
			f32 deltaTime = 0.001f;
			g_CustomSystem->Update(deltaTime);
			g_RenderingSystem->Update(deltaTime);
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