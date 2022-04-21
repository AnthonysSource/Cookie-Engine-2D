#include "Components.h"
#include "CookieEngine.h"
#include "Systems.h"



namespace Cookie::Application {

	using namespace Cookie;

	void CreateControllableCookie(EntityAdmin *const EntitiesAdmin, Float3 pos) {
		EntityID e = EntitiesAdmin->CreateEntity();

		TransformComponent transform = TransformComponent{};
		RotatingComponent rotating{};
		MoveComponent move = MoveComponent{};
		RenderComponent render = RenderComponent{};

		transform.m_Position = pos;
		rotating.m_Speed = 5.0f;
		move.m_Speed = 4.0f;
		render.m_SpriteID = 4;

		EntitiesAdmin->AddComponent(e, transform);
		EntitiesAdmin->AddComponent(e, render);
		EntitiesAdmin->AddComponent(e, rotating);
		EntitiesAdmin->AddComponent(e, move);
	};

	void CreateFloatingButterStick(EntityAdmin *const EntitiesAdmin, Float3 pos) {
		EntityID e = EntitiesAdmin->CreateEntity();

		TransformComponent transform = TransformComponent{};
		RotatingComponent rotating = RotatingComponent{};
		FloatComponent floating = FloatComponent{};
		RenderComponent render = RenderComponent{};

		transform.m_Position = pos;
		rotating.m_Speed = 3.14f;
		floating.m_Speed = 1.0f;
		floating.m_Amplitude = 0.25f;
		render.m_SpriteID = 5;

		EntitiesAdmin->AddComponent(e, transform);
		EntitiesAdmin->AddComponent(e, render);
		EntitiesAdmin->AddComponent(e, rotating);
		EntitiesAdmin->AddComponent(e, floating);
	}

	void CreateSugarCube(EntityAdmin *const EntitiesAdmin, Float3 pos) {
		EntityID e = EntitiesAdmin->CreateEntity();

		TransformComponent transform = TransformComponent{};
		RenderComponent render = RenderComponent{};

		transform.m_Position = pos;
		render.m_SpriteID = 3;

		EntitiesAdmin->AddComponent(e, transform);
		EntitiesAdmin->AddComponent(e, render);
	}

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
		CreateSugarCube(EntitiesAdmin, Float3(-2.0f, 0.0f, 0.0f));
		CreateControllableCookie(EntitiesAdmin, Float3(0.0f, 0.0f, 0.0f));
		CreateFloatingButterStick(EntitiesAdmin, Float3(2.0f, 0.0f, 0.0f));
	}

} // namespace Cookie::Application

int main() {
	Cookie::Application::Run(Cookie::Application::CreateWorld);
	return 0;
}