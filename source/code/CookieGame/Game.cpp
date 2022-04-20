#include "Components.h"
#include "CookieEngine.h"
#include "Systems.h"

namespace Cookie::Application {

	void CreateWorld(EntityAdmin *const EntitiesAdmin) {

		// Register Components
		EntitiesAdmin->RegisterComponent<RotatingComponent>();
		EntitiesAdmin->RegisterComponent<FloatComponent>();
		EntitiesAdmin->RegisterComponent<MoveComponent>();

		// Register Systems in order of execution
		EntitiesAdmin->RegisterSystem<RotateSystem>();
		EntitiesAdmin->RegisterSystem<FloatSystem>();
		EntitiesAdmin->RegisterSystem<MoveSystem>();

		// Create World Entities
		EntityID e = EntitiesAdmin->CreateEntity();
		TransformComponent t{};
		t.m_Position = Float3(-2.0f, 0.0f, 0.0f);
		EntitiesAdmin->AddComponent(e, RenderComponent{});
		EntitiesAdmin->AddComponent(e, t);

		e = EntitiesAdmin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = Float3(0.0f, 0.0f, 0.0f);
		RotatingComponent r{};
		r.m_Speed = 5.0f;
		MoveComponent m = MoveComponent{};
		m.m_Speed = 4.0f;
		EntitiesAdmin->AddComponent(e, t);
		EntitiesAdmin->AddComponent(e, RenderComponent{});
		EntitiesAdmin->AddComponent(e, r);
		EntitiesAdmin->AddComponent(e, m);

		e = EntitiesAdmin->CreateEntity();
		t = TransformComponent{};
		t.m_Position = Float3(2.0f, 0.0f, 0.0f);
		r = RotatingComponent{};
		r.m_Speed = 3.14f;
		FloatComponent f = FloatComponent{};
		f.m_Speed = 1.0f;
		f.m_Amplitude = 0.25f;
		EntitiesAdmin->AddComponent(e, t);
		EntitiesAdmin->AddComponent(e, r);
		EntitiesAdmin->AddComponent(e, RenderComponent{});
		EntitiesAdmin->AddComponent(e, f);
	}

} // namespace Cookie::Application

int main() {
	Cookie::Application::Run(Cookie::Application::CreateWorld);
	return 0;
}