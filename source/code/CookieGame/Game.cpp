#include "Components.h"
#include "CookieEngine.h"
#include "Systems.h"

namespace Cookie::Application {

	void CreateWorld() {

		using namespace Cookie::Application;

		g_Admin->RegisterComponent<RotatingComponent>();
		g_Admin->RegisterComponent<FloatComponent>();

		g_Admin->RegisterSystem<RotateSystem>();
		g_Admin->RegisterSystem<FloatSystem>();

		EntityID e = g_Admin->CreateEntity();
		TransformComponent t{};
		t.m_Position = Float3(-2.0f, 0.0f, 0.0f);
		g_Admin->AddComponent(e, RenderComponent{});
		g_Admin->AddComponent(e, t);

		e = g_Admin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = Float3(0.0f, 0.0f, 0.0f);
		RotatingComponent r{};
		r.m_Speed = 5.0f;
		g_Admin->AddComponent(e, t);
		g_Admin->AddComponent(e, RenderComponent{});
		g_Admin->AddComponent(e, r);

		e = g_Admin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = Float3(2.0f, 0.0f, 0.0f);
		r = RotatingComponent{};
		r.m_Speed = 3.14f;
		FloatComponent f = FloatComponent{};
		f.m_Speed = 1.0f;
		f.m_Amplitude = 0.25f;
		g_Admin->AddComponent(e, t);
		g_Admin->AddComponent(e, r);
		g_Admin->AddComponent(e, RenderComponent{});
		g_Admin->AddComponent(e, f);
	}

} // namespace Cookie::Application

int main() {
	Cookie::Application::Run(Cookie::Application::CreateWorld);
	return 0;
}