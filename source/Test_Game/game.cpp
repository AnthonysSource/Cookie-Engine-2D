#include "CookieEngine.h"

using namespace Cookie;

int main() {
	// ----
	// Create the game world, all the following entities will be instantiated in the current world
	// ----
	// WorldID world = Game::CreateWorld();
	// Game::SetCurrentWorld(world);

	// ----
	// Create the entity definitions, these are like templates to create entities
	// ----
	// EntityDefinition camDef
	// camDef.AddComponent(CameraComponent)
	// EntityDefinition playerDef
	// playerDef.AddComponent(TransformComponent(){ position = (0,2,1) } )
	// ScriptComponent sc;

	// ----
	// ScriptComponent is a way to implement the classic gameobject model
	// when needed, its especially useful when trying to implement game logic
	// like inventory systems or an AI behaviour tree because that type of logic
	// usually doesn't fit very well in systems
	// ----
	// sc.onCreate = UserCreateFunc
	// sc.onUpdate = UserUpdateFunc
	// playerDef.AddComponent(sc)
	// SpriteID sp = Resources::LoadSprite("player/player.png");
	// playerDef.AddComponent(RenderComponent(){ m_SpriteID = sp })

	// ----
	// Use definitions to create entities and override data from the template if wanted
	// ----
	// EntityID cam = Entities::Create(camDef);
	// auto t = Entities.GetComponent<TransformComponent>(cam);
	// t.m_Position = (0,0,1);
	//
	// EntityID p1 = Entities::CreateEntity(playerDef);
	// EntityID p2 = Entities::CreateEntity(playerDef);
	// EntityID p3 = Entities::CreateEntity(playerDef);
	// EntityID p4 = Entities::CreateEntity(playerDef);
	//
	// auto t = Entities.GetComponent<TransformComponent>(p1);
	// t.m_Position = (-1,0,0);
	// auto t = Entities.GetComponent<TransformComponent>(p2);
	// t.m_Position = (2,0,0);
	// auto t = Entities.GetComponent<TransformComponent>(p3);
	// auto r = Entities.GetComponent<RenderComponent>(p3);
	// t.m_Position = (0,2,0);
	// r.m_SpriteID = sprite2;

	// ----
	// Add User-defined Systems, these sistems are classes that inherit from the base system class
	// TODO: System executing order/priority
	// ----
	// Systems::Add(FogSystem())
	// Systems::Add(EffectsSystem())
	// 

	Application::Run();
}