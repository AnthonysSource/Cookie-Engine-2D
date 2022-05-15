#include "Components.h"
#include "CookieEngine.h"
#include "Systems.h"

using namespace Cookie;

// Sprites
SpriteHandle cookieSprite;
SpriteHandle bigCookieSprite;

// Entities Templates
void CreatePlayer(EntityAdmin *const EntitiesAdmin, Float3 pos) {
	EntityID e = EntitiesAdmin->CreateEntity();

	TransformComponent transform = {};
	RenderComponent render = {};
	PlayerCharacterComponent move = {};
	AttackComponent attack = {};

	attack.m_Area = 1.5f;
	attack.m_AttackDamage = 10.0f;
	attack.m_CooldownTotal = 1.0f;
	attack.m_CooldownElapsed = 0.0f;

	transform.m_Position = pos;
	render.m_SpriteHandle = bigCookieSprite;
	move.m_Speed = 6.0f;

	EntitiesAdmin->AddComponent(e, transform);
	EntitiesAdmin->AddComponent(e, render);
	EntitiesAdmin->AddComponent(e, move);
	EntitiesAdmin->AddComponent(e, attack);
};

void CreateEnemy(EntityAdmin *const EntitiesAdmin, Float3 pos) {
	EntityID e = EntitiesAdmin->CreateEntity();

	TransformComponent transform = {};
	RenderComponent render = {};
	EnemyComponent enemy = {};

	transform.m_Position = pos;
	render.m_SpriteHandle = cookieSprite;
	enemy.m_Acceleration = Random::Float(1.0f, 3.0f);
	enemy.m_TopSpeed = Random::Float(3.0f, 7.0f);

	EntitiesAdmin->AddComponent(e, transform);
	EntitiesAdmin->AddComponent(e, render);
	EntitiesAdmin->AddComponent(e, enemy);
};

void LoadResources() {
	CKE_PROFILE_EVENT();
	// Resources
	ImageHandle cookieImg = ResourcesSystem::LoadImage("cookie.png");
	ImageHandle butterImg = ResourcesSystem::LoadImage("butter_stick.png");
	ImageHandle sugarImg = ResourcesSystem::LoadImage("sugar_cube.png");

	cookieSprite = ResourcesSystem::GenerateSprite(cookieImg, 32.0f * 8.0f);
	bigCookieSprite = ResourcesSystem::GenerateSprite(cookieImg, 32.0f * 2.5f);
}

void RegisterComponents(EntityAdmin *const EntitiesAdmin) {

	// Register Components
	EntitiesAdmin->RegisterComponent<PlayerCharacterComponent>();
	EntitiesAdmin->RegisterComponent<EnemyComponent>();
	EntitiesAdmin->RegisterComponent<AttackComponent>();

	EntitiesAdmin->RegisterSinglComponent<SinglMainPlayerComponent>();

	// Register Systems in order of execution
	EntitiesAdmin->RegisterSystem<PlayerMovementSystem>();
	EntitiesAdmin->RegisterSystem<AttackSystem>();
	EntitiesAdmin->RegisterSystem<EnemySystem>();
}

// Game World Init
void CreateWorld(EntityAdmin *const EntitiesAdmin) {
	CKE_PROFILE_EVENT();
	using namespace Cookie;

	// Create World Entities
	CreatePlayer(EntitiesAdmin, Float3(-4.0f, 0.0f, -0.001f));

	i32 rows = 1000;
	i32 columns = 100;
	for (size_t x = 0; x < columns; x++) {
		for (size_t y = 0; y < rows; y++) {
			CreateEnemy(EntitiesAdmin, Float3(-0.5f + (1.0f / (f32)columns) * x, -0.5f + (1.0f / (f32)rows) * y, -0.001f));
		}
	}
}

int main() {
	using namespace Cookie;
	GameInitData g = {};

	WindowDescription wd = {};
	wd.m_Title = "Cookie Survival";
	wd.m_Width = 1280;
	wd.m_Height = 720;
	wd.m_IsFullScreen = false;
	wd.m_RefreshRate = 144;

	g.m_WindowDesc = wd;
	g.m_LoadResourcesFunc = LoadResources;
	g.m_CreateWorldFunc = CreateWorld;
	g.m_RegisterECSFunc = RegisterComponents;

	Application::Run(&g);
	return 0;
}