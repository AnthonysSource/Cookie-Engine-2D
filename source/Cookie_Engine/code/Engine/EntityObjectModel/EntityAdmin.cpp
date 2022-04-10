#include "EntityAdmin.h"

namespace Cookie {

std::vector<TransformComponent> m_Transforms;
CameraComponent m_Camera;
InputComponent m_Input;

const u32 ENTITIES_AMOUNT = 10;

void GenerateCookies() {
	m_Camera.m_Forward = vec3(0.0f, 0.0f, 1.0f);
	m_Camera.m_Position = vec3(0.0f, 0.0f, 0.0f);
	m_Camera.m_Up = vec3(0.0f, 1.0f, 0.0f);

	for (size_t i = 0; i < ENTITIES_AMOUNT; i++) {
		TransformComponent t{};
		t.m_Position = vec3(-5.0f + 1.0f * i, 0.0f, 0.0f);
		m_Transforms.push_back(t);
	}
}

void UpdateCookies() {
	for (size_t i = 0; i < m_Transforms.size(); i++) {
		m_Transforms[i].m_Position += vec3(0.0f, 1.0f, 0.0f) * Random::Float(-0.015f, 0.015f);
		m_Transforms[i].m_Rotation += vec3(0.0f, 0.05f, 0.0f) * (float)Random::Int(0, 3);
	}
}

void ClearCookies() { m_Transforms.clear(); }

//
// ----- ECS
//

void EntityAdmin::Init() {
	for (EntityID entityID = 0; entityID < MAX_ENTITIES; ++entityID) {
		m_AvailableEntityIDs.push(entityID);
	}
}

EntityID EntityAdmin::CreateEntity() {
	COOKIE_ASSERT(m_ActiveEntitiesCount < MAX_ENTITIES, "Trying to create too many entities");

	// Pop an entity id from the queue
	EntityID id = m_AvailableEntityIDs.front();
	m_AvailableEntityIDs.pop();
	++m_ActiveEntitiesCount;

	return id;
}

void EntityAdmin::DestroyEntity(EntityID entityID) {
	// Mark the entity id as available
	m_AvailableEntityIDs.push(entityID);
	--m_ActiveEntitiesCount;

	// Clear entity signature
	m_Signatures[entityID].reset();

	// Destroy entity components
	for (auto const &pair : m_ComponentArrays) {
		IComponentArray *array = pair.second;
		array->DestroyEntity(entityID);
	}

	// Erase entity from systems iteration cache
	for (auto const &pair : m_Systems) {
		System *system = pair.second;
		system->m_EntitiesCache.erase(entityID);
	}
}

} // namespace Cookie