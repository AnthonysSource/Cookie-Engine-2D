#include "Application.h"

#include "Core/Common.h"
#include "Core/Logging/Log.h"
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

struct FloatComponent {
	f32 m_Speed = 2.0f;
	f32 m_Amplitude = 2.0f;
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

class FloatSystem : public System {
  public:
	void InitSignature() {
		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<FloatComponent>(), true);
	}

	void Update(f32 dt) override {
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			FloatComponent *f = g_Admin->GetComponent<FloatComponent>(entityID);
			t->m_Position.y = f->m_Amplitude * cos(f->m_Speed * glfwGetTime());
		}
	}
};

namespace Application {

	Window window;
	RenderingSystem *g_RenderingSystem = new RenderingSystem();
	RotateSystem *g_RotateSystem = new RotateSystem();
	FloatSystem *g_FloatSystem = new FloatSystem();

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
		g_Admin->RegisterComponent<FloatComponent>();
		g_Admin->RegisterSystem(g_RenderingSystem);
		g_Admin->RegisterSystem(g_RotateSystem);
		g_Admin->RegisterSystem(g_FloatSystem);

		// Create Entities
		EntityID e = g_Admin->CreateEntity();
		TransformComponent t{};
		t.m_Position = vec3(-2.0f, 0.0f, 0.0f);
		g_Admin->AddComponent(e, t);

		e = g_Admin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = vec3(0.0f, 0.0f, 0.0f);
		RotatingComponent r{};
		r.m_Speed = 5.0f;
		g_Admin->AddComponent(e, t);
		g_Admin->AddComponent(e, r);

		e = g_Admin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = vec3(2.0f, 0.0f, 0.0f);
		r = RotatingComponent{};
		r.m_Speed = 3.14f;
		FloatComponent f = FloatComponent{};
		f.m_Speed = 1.0f;
		f.m_Amplitude = 0.25f;
		g_Admin->AddComponent(e, t);
		g_Admin->AddComponent(e, r);
		g_Admin->AddComponent(e, f);

		CKE_LOG_INFO("Initializing Rendering System");
		g_RenderingSystem->Init();

		CKE_LOG_INFO("Starting engine loop");
		f32 tLastFrame = glfwGetTime();
		f32 tCurrentFrame = tLastFrame;

		while (Platform::IsRunning(window.m_Window)) {
			tCurrentFrame = glfwGetTime();
			f64 deltaTime = tCurrentFrame - tLastFrame;
			tLastFrame = tCurrentFrame;

			InputSystem::Update();
			g_RotateSystem->Update(deltaTime);
			g_FloatSystem->Update(deltaTime);
			g_RenderingSystem->Update(deltaTime);
		}

		// Shutdown
		CKE_LOG_INFO("Shutting down");
		g_RenderingSystem->Shutdown();
		InputSystem::Shutdown();
		Platform::Shutdown();

		delete g_Admin;
		delete g_RenderingSystem;
		delete g_RotateSystem;
		delete g_FloatSystem;
	}

} // namespace Application
} // namespace Cookie