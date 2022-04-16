#pragma once

#include "Core/Defines.h"
#include "Core/Types/Containers.h"
#include "Core/Types/PrimitiveTypes.h"

#include "Core/Math.h"
#include "Core/Random/Random.h"

#include "Entities/BaseComponents.h"

namespace Cookie {

// -----------------------------------------------------------------

// We should use generational id's because this approach
// could generate problems when destroying and creating a
// vast amount of entitites in a short timespan
using EntityID = u32;

using ComponentSignatureIndex = u8;

const EntityID MAX_ENTITIES = 5000;
const ComponentSignatureIndex MAX_COMPONENTS = 128;

using Signature = Bitset<MAX_COMPONENTS>;

// -----------------------------------------------------------------

class IComponentArray {
  public:
	virtual ~IComponentArray() = default;
	virtual void DestroyEntity(EntityID entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
  public:
	void Insert(EntityID entity, T component) {
		CKE_ASSERT(m_EntityToIndex.find(entity) == m_EntityToIndex.end(),
					  "Component added to same entity more than once");

		u64 newIndex = m_Count;
		m_EntityToIndex[entity] = newIndex;
		m_IndexToEntity[newIndex] = entity;
		m_Array[newIndex] = component;
		++m_Count;
	}

	void Remove(EntityID entity) {
		u64 indexOfRemovedEntity = m_EntityToIndex[entity];
		u64 indexOfLastElement = m_Count - 1;
		m_Array[indexOfRemovedEntity] = m_Array[indexOfLastElement];

		EntityID entityOfLastElement = m_IndexToEntity[indexOfLastElement];
		m_EntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndex.erase(entity);
		m_IndexToEntity.erase(indexOfLastElement);

		--m_Count;
	}

	T *Get(EntityID entity) { return &m_Array[m_EntityToIndex[entity]]; }

	void DestroyEntity(EntityID entity) override {
		if (m_EntityToIndex.find(entity) != m_EntityToIndex.end()) {
			Remove(entity);
		}
	}

  private:
	T m_Array[MAX_ENTITIES];
	u64 m_Count;

	std::unordered_map<EntityID, u64> m_EntityToIndex;
	std::unordered_map<u64, EntityID> m_IndexToEntity;
};

// -----------------------------------------------------------------

class EntityAdmin;

class System {
  public:
	virtual void InitSignature() = 0;
	virtual void Update(f32 dt) = 0;

  protected:
	Signature m_Signature{};
	TSet<EntityID> m_EntitiesCache{};

	friend class EntityAdmin;
};

class EntityAdmin {
  public:
	void Init();

	EntityID CreateEntity();
	void DestroyEntity(EntityID entity);

	template <typename T> void RegisterComponent() {
		u32 typeID = typeid(T).hash_code();
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

	template <typename T> T *GetComponent(EntityID entity) {
		return GetComponentArray<T>()->Get(entity);
	};

	template <typename T> ComponentSignatureIndex GetComponentSignatureID() {
		u32 typeID = typeid(T).hash_code();
		return m_ComponentSignatureIndex[typeID];
	};

	template <typename T> void RegisterSystem(T *system) {
		u32 typeID = typeid(T).hash_code();
		// System *system = new T();
		m_Systems.insert({typeID, system});
		system->InitSignature();
	};

  private:
	TQueue<EntityID> m_AvailableEntityIDs{};
	TArray<Signature, MAX_ENTITIES> m_Signatures{};
	u32 m_ActiveEntitiesCount{};

	THashMap<u32, IComponentArray *> m_ComponentArrays{};
	THashMap<u32, ComponentSignatureIndex> m_ComponentSignatureIndex{};
	ComponentSignatureIndex m_NextComponentIndex{};

	THashMap<u32, System *> m_Systems{};

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
		return dynamic_cast<ComponentArray<T> *>(m_ComponentArrays[typeID]);
	}
};

} // namespace Cookie