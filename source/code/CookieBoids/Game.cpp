#include "Components.h"
#include "CookieEngine.h"
#include "Systems.h"

using namespace Cookie;

// Sprites
SpriteHandle cookieSprite;
SpriteHandle butterSprite;
SpriteHandle sugarSprite;

// Entities Templates
void CreateCookieBoid(EntityAdmin *const EntitiesAdmin, Float3 pos) {
	EntityID e = EntitiesAdmin->CreateEntity();

	TransformComponent transform = {};
	RenderComponent render = {};
	BoidComponent boid = {};

	transform.m_Position = pos;
	render.m_SpriteHandle = cookieSprite;

	EntitiesAdmin->AddComponent(e, transform);
	EntitiesAdmin->AddComponent(e, render);
	EntitiesAdmin->AddComponent(e, boid);
};

void LoadResources() {
	CKE_PROFILE_EVENT();
	// Resources
	ImageHandle cookieImg = ResourcesSystem::LoadImage("cookie.png");
	ImageHandle butterImg = ResourcesSystem::LoadImage("butter_stick.png");
	ImageHandle sugarImg = ResourcesSystem::LoadImage("sugar_cube.png");

	cookieSprite = ResourcesSystem::GenerateSprite(cookieImg, 32.0f * 8.0f);
	butterSprite = ResourcesSystem::GenerateSprite(butterImg, 96.0f);
	sugarSprite = ResourcesSystem::GenerateSprite(sugarImg, 96.0f);
}

// Game World Init
void CreateWorld(EntityAdmin *const EntitiesAdmin) {
	CKE_PROFILE_EVENT();
	using namespace Cookie;

	// Register Components
	EntitiesAdmin->RegisterComponent<BoidComponent>();

	// Register Systems in order of execution
	EntitiesAdmin->RegisterSystem<BoidsSystem>();

	// Create World Entities
	i32 rows = 25;
	i32 columns = 10;
	for (size_t x = 0; x < columns; x++) {
		for (size_t y = 0; y < rows; y++) {
			CreateCookieBoid(EntitiesAdmin, Float3(-0.5f + (1.0f / (f32)columns) * x, -0.5f + (1.0f / (f32)rows) * y, -0.001f));
		}
	}
}

int main() {
	using namespace Cookie;
	GameInitData g = {};

	WindowDescription wd = {};
	wd.m_Title = "Cookie Boids";
	wd.m_Width = 1280;
	wd.m_Height = 720;
	wd.m_IsFullScreen = false;
	wd.m_RefreshRate = 144;

	g.m_WindowDesc = wd;
	g.m_LoadResourcesFunc = LoadResources;
	g.m_CreateWorldFunc = CreateWorld;

	Application::Run(&g);
	return 0;
}