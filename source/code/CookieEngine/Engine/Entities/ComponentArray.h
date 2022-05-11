#pragma once

#include "Core/Types/PrimitiveTypes.h"

#include "Common.h"

namespace Cookie {

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void DestroyEntity(EntityID entity) = 0;
	};

	template <typename T> class ComponentArray : public IComponentArray {
	public:
		void Insert(EntityID entity, T component) {
			CKE_ASSERT(m_EntityToIndex.find(entity) == m_EntityToIndex.end(), "Component added to same entity more than once");

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

		T *At(size_t index) { return &m_Array[index]; }

		u64 Count() { return m_Count; }

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

} // namespace Cookie