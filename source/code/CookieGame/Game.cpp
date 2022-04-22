#include "Components.h"
#include "CookieEngine.h"
#include "Systems.h"

namespace Cookie::Application {

	using namespace Cookie;

	// Sprites
	SpriteHandle cookieSprite;
	SpriteHandle butterSprite;
	SpriteHandle sugarSprite;

	// Entities Templates
	void CreateControllableCookie(EntityAdmin *const EntitiesAdmin, Float3 pos) {
		EntityID e = EntitiesAdmin->CreateEntity();

		TransformComponent transform = TransformComponent{};
		MoveComponent move = MoveComponent{};
		RotatingComponent rotating = RotatingComponent{};
		RenderComponent render = RenderComponent{};

		transform.m_Position = pos;
		rotating.m_Speed = 3.14f;
		move.m_Speed = 4.0f;
		render.m_SpriteHandle = cookieSprite;

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
		render.m_SpriteHandle = butterSprite;

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
		render.m_SpriteHandle = sugarSprite;

		EntitiesAdmin->AddComponent(e, transform);
		EntitiesAdmin->AddComponent(e, render);
	}

	// Game World Init
	void CreateWorld(EntityAdmin *const EntitiesAdmin) {

		// Resources
		ImageHandle cookieImg = ResourcesSystem::Load("cookie.png");
		ImageHandle butterImg = ResourcesSystem::Load("butter_stick.png");
		ImageHandle sugarImg = ResourcesSystem::Load("sugar_cube.png");

		cookieSprite = ResourcesSystem::GenerateSprite(cookieImg, 32.0f);
		butterSprite = ResourcesSystem::GenerateSprite(butterImg, 32.0f);
		sugarSprite = ResourcesSystem::GenerateSprite(sugarImg, 32.0f);

		// Register Components
		EntitiesAdmin->RegisterComponent<RotatingComponent>();
		EntitiesAdmin->RegisterComponent<FloatComponent>();
		EntitiesAdmin->RegisterComponent<MoveComponent>();

		// Register Systems in order of execution
		EntitiesAdmin->RegisterSystem<RotateSystem>();
		EntitiesAdmin->RegisterSystem<FloatSystem>();
		EntitiesAdmin->RegisterSystem<MoveSystem>();

		// Create World Entities
		CreateFloatingButterStick(EntitiesAdmin, Float3(-2.0f, 0.0f, 0.0f));
		CreateControllableCookie(EntitiesAdmin, Float3(0.0f, 0.0f, 0.0f));
		CreateControllableCookie(EntitiesAdmin, Float3(2.0f, 2.0f, 0.0f));
		CreateControllableCookie(EntitiesAdmin, Float3(2.0f, -2.0f, 0.0f));
		CreateControllableCookie(EntitiesAdmin, Float3(-2.0f, 2.0f, 0.0f));
		CreateControllableCookie(EntitiesAdmin, Float3(-2.0f, -2.0f, 0.0f));
		CreateSugarCube(EntitiesAdmin, Float3(2.0f, 0.0f, 0.0f));
	}

} // namespace Cookie::Application

int main() {
	Cookie::Application::Run(Cookie::Application::CreateWorld);
	return 0;
}