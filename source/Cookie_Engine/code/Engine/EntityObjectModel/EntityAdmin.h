#pragma once

#include "Core/Common.h"
#include "Core/Logging/Logging.h"
#include "Core/Math.h"
#include "Core/Random/Random.h"

// Remove this in the future
#include "Core/InputSystem/InputSystem.h"

#include <stdlib.h>

namespace Cookie {
namespace Object {

	struct TransformComponent {
		vec3 m_Position;
		vec3 m_Rotation;
		vec3 m_Scale;
	};

	struct RenderComponent {
		u32 m_SpriteID;
	};

	struct CameraComponent {
		vec3 m_Position;
		vec3 m_Forward;
		vec3 m_Up;
	};

	struct ScriptComponent {};

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

	// ---------------------------------------------------
	// We should use generational id's because this approach
	// could generate problems when destroying and creating a
	// vast amount of entitites in a short timespan
	using EntityID = u32;
	const EntityID MAX_ENTITIES = 5000;

	using ComponentSignatureIndex = u8;
	const ComponentSignatureIndex MAX_COMPONENTS = 128;

	using Signature = std::bitset<MAX_COMPONENTS>;

	class IComponentArray {
	  public:
		virtual ~IComponentArray() = default;
		virtual void DestroyEntity(EntityID entity) = 0;
	};

	template <typename T> class ComponentArray : public IComponentArray {
	  public:
		void Insert(EntityID entity, T component);
		void Remove(EntityID entity, T component);
		T *Get(EntityID entity);

	  private:
		T m_Array[MAX_ENTITIES];
		u64 m_Count;

		std::unordered_map<EntityID, u64> m_EntityToIndex;
		std::unordered_map<u64, EntityID> m_IndexToEntity;
	};

	class EntityAdmin;

	class System {
	  public:
		virtual void Init(Signature signature);
		virtual void Update(f32 dt);

	  protected:
		Signature m_Signature;
		std::set<EntityID> m_EntitiesCache;

		friend class EntityAdmin;
	};

	class EntityAdmin {
	  public:
		EntityID CreateEntity();
		void DestroyEntity(EntityID entity);

		template <typename T> void RegisterComponent() {
			u32 typeID = typeid(T).hash_code();
			ComponentArray<T> *array = new ComponentArray<T>();
			m_ComponentArrays.insert(typeID, array);
			++m_ComponentSignatureIndex++;
		};

		template <typename T> void AddComponent(EntityID entity, T component) {
			GetComponentArray<T>()->Insert(entity, component);

			auto signature = m_Signatures[entity];
			signature.set(GetComponentSignatureID<T>(), true);
			m_Signatures[entity] = signature;

			EntitySignatureChanged(entity)
		}

		template <typename T> void RemoveComponent(EntityID entity) {
			GetComponentArray<T>()->Remove(entity);

			auto signature = m_Signatures[entity];
			signature.set(GetComponentSignatureID<T>(), false);
			m_Signatures[entity] = signature;

			EntitySignatureChanged(entity)
		}

		template <typename T> T *GetComponent(EntityID entitiy) {
			return GetComponentArray<T>()->Get(entity);
		}

		template <typename T> ComponentSignatureIndex GetComponentSignatureID() {
			u32 typeID = typeid(T).hash_code();
			return m_ComponentSignatureIndex[typeID];
		};

		template <typename T> void RegisterSystem() {
			u32 typeID = typeid(T).hash_code();
			System *system = new T();
			m_Systems.insert({typeID, system});
		};

	  private:
		std::queue<EntityID> m_AvailableEntityIDs;
		std::array<Signature, MAX_ENTITIES> m_Signatures;
		u32 m_ActiveEntitiesCount;

		std::unordered_map<u32, IComponentArray *> m_ComponentArrays{};
		std::unordered_map<u32, ComponentSignatureIndex> m_ComponentSignatureIndex{};
		ComponentSignatureIndex m_NextComponentIndex{};

		std::unordered_map<u32, System *> m_Systems{};

		void EntitySignatureChanged(EntityID entityID) {
			Signature entitySignature = m_Signatures[entityID];

			for (auto const &pair : m_Systems) {
				u32 systemTypeID = pair.first;
				System *system = pair.second;

				if ((entitySignature & system->m_Signature) == system->m_Signature) {
					// We don't have to check if its already added
					// because we are using a set
					system->m_EntitiesCache.insert(entityID);
				} else {
					system->m_EntitiesCache.erase(entityID);
				}
			}
		}

		template <typename T> ComponentArray<T> *GetComponentArray() {
			u32 typeID = typeid(T).hash_code();
			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeID]);
		}
	};

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

} // namespace Object
} // namespace Cookie