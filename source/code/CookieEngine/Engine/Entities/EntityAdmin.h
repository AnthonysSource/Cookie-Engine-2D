#pragma once

#include "Core/Defines.h"
#include "Core/Types/Containers.h"
#include "Core/Types/PrimitiveTypes.h"
#include "Core/Math/Math.h"
#include "Core/Random/Random.h"

#include "Entities/Common.h"
#include "Entities/ComponentArray.h"
#include "Entities/System.h"

namespace Cookie {

	// -----------------------------------------------------------------

	class System;

	class EntityAdmin {

	public:
		void Init();
		void Update(f32 deltaTime);
		void Shutdown();

		// Entities
		// -----------------------------------------------------------------

		EntityID CreateEntity();
		void DestroyEntity(EntityID entity);

		// Singleton Components
		// -----------------------------------------------------------------

		template <typename T> void RegisterSinglComponent() {
			size_t typeID = typeid(T).hash_code();
			T *singlComp = new T();
			m_SingletonComponents.insert({typeID, (void *)singlComp});
		};

		template <typename T> T *GetSinglComponent() {
			size_t typeID = typeid(T).hash_code();
			CKE_ASSERT(m_SingletonComponents.find(typeID) != m_SingletonComponents.end(),
					   "Trying to access a singleton component that doesn't exist");
			return (T *)(m_SingletonComponents[typeID]);
		}

		// Components
		// -----------------------------------------------------------------

		template <typename T> void RegisterComponent() {
			size_t typeID = typeid(T).hash_code();
			ComponentArray<T> *array = new ComponentArray<T>();
			m_ComponentArrays.insert({typeID, array});
			m_ComponentSignatureIndex.insert({typeID, m_NextComponentIndex});
			++m_NextComponentIndex;
		};

		template <typename T> void AddComponent(EntityID entity, T component) {
			GetComponentArray<T>()->Insert(entity, component);

			auto signature = m_Signatures[entity];
			signature.set(GetComponentSignatureID<T>(), true);
			m_Signatures[entity] = signature;

			EntitySignatureChanged(entity);
		};

		template <typename T> void RemoveComponent(EntityID entity) {
			GetComponentArray<T>()->Remove(entity);

			auto signature = m_Signatures[entity];
			signature.set(GetComponentSignatureID<T>(), false);
			m_Signatures[entity] = signature;

			EntitySignatureChanged(entity);
		};

		template <typename T> T *GetComponent(EntityID entity) { return GetComponentArray<T>()->Get(entity); };

		template <typename T> ComponentArray<T> *GetComponentArray() {
			size_t typeID = typeid(T).hash_code();
			CKE_ASSERT(m_ComponentArrays.find(typeID) != m_ComponentArrays.end(), "Trying to add an unregistered component to an entity");
			return dynamic_cast<ComponentArray<T> *>(m_ComponentArrays[typeID]);
		}

		// Signature
		// -----------------------------------------------------------------

		template <typename T> ComponentSignatureIndex GetComponentSignatureID() {
			size_t typeID = typeid(T).hash_code();
			CKE_ASSERT(m_ComponentSignatureIndex.find(typeID) != m_ComponentSignatureIndex.end(),
					   "A system is trying to acess a component that is not registered");
			return m_ComponentSignatureIndex[typeID];
		};

		// Systems
		// -----------------------------------------------------------------

		template <typename T> void RegisterSystem() {
			size_t typeID = typeid(T).hash_code();
			System *system = new T();
			system->m_Admin = this;
			m_Systems.insert({typeID, system});
			system->InitSignature();
		};

		template <typename T> void RegisterSystem(T *system) {
			size_t typeID = typeid(T).hash_code();
			m_Systems.insert({typeID, system});
			system->InitSignature();
		};

	private:
		TQueue<EntityID> m_AvailableEntityIDs{};
		// TArray<Signature, MAX_ENTITIES> m_Signatures{};
		TVector<Signature> m_Signatures{};
		u32 m_ActiveEntitiesCount{};

		THashMap<size_t, IComponentArray *> m_ComponentArrays{};
		THashMap<size_t, void *> m_SingletonComponents{};
		THashMap<size_t, ComponentSignatureIndex> m_ComponentSignatureIndex{};
		ComponentSignatureIndex m_NextComponentIndex{};

		THashMap<size_t, System *> m_Systems{};

		// -----------------------------------------------------------------

		void EntitySignatureChanged(EntityID entityID) {
			Signature entitySignature = m_Signatures[entityID];

			for (auto const &pair : m_Systems) {
				size_t systemTypeID = pair.first;
				System *system = pair.second;

				if ((entitySignature & system->m_Signature) == system->m_Signature) {
					// We don't have to check if its already added
					// because we are using a set
					system->m_Entities.insert(entityID);
				} else {
					system->m_Entities.erase(entityID);
				}
			}
		}
	};

} // namespace Cookie