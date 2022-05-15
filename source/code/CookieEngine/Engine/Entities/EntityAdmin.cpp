#include "EntityAdmin.h"
#include "Core/Profiling/Profiling.h"

#include "Entities/BaseComponents.h"

namespace Cookie {

	void EntityAdmin::Init() {
		// Create Entity IDs
		for (EntityID entityID = 0; entityID < MAX_ENTITIES; ++entityID) {
			m_AvailableEntityIDs.push(entityID);
			m_Signatures.emplace_back(Signature{});
		}

		// Register Base Components
		RegisterComponent<TransformComponent>();
		RegisterComponent<RenderComponent>();
		RegisterComponent<PhysicsComponent>();
		RegisterComponent<CameraComponent>();

		RegisterSinglComponent<InputComponent>();
		RegisterSinglComponent<CameraComponentSingl>();
	}

	void EntityAdmin::Update(f32 deltaTime) {
		CKE_PROFILE_EVENT();

		// Update all systems
		for (size_t i = 0; i < m_SystemsExecutionOrder.size(); i++) {
			m_Systems[m_SystemsExecutionOrder[i]]->Update(deltaTime);
		}
	}

	void EntityAdmin::Shutdown() {
		// Deallocate all systems
		for (auto const pair : m_Systems) {
			delete pair.second;
		}
		// Deallocate all component arrays
		for (auto const pair : m_ComponentArrays) {
			delete pair.second;
		}
	}

	EntityID EntityAdmin::CreateEntity() {
		CKE_ASSERT(m_ActiveEntitiesCount < MAX_ENTITIES, "Trying to create too many entities");

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
			system->m_Entities.erase(entityID);
		}
	}

} // namespace Cookie