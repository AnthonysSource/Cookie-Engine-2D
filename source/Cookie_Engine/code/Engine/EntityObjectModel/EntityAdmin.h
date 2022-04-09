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
	using EntityID = u32;
	const EntityID MAX_ENTITIES = 5000;

	using ComponentType = u8;
	const ComponentType MAX_COMPONENTS = 128;

	using Signature = std::bitset<MAX_COMPONENTS>;

	class IComponentArray {
	  public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(EntityID entity) = 0;
	};

	class System {
	  public:
		virtual void Init(Signature signature);
		virtual void Update(f32 dt);

	  protected:
		Signature m_Signature;
	};

	class EntityAdmin {
	  public:
		void CreateEntity();
		void DestroyEntity(EntityID entity);

		template <typename T> void AddComponent(EntityID entity, T component);
		template <typename T> void RemoveComponent(EntityID entity);
		template <typename T> T *GetComponent(EntityID entitiy);
		template <typename T> ComponentType GetComponentType();

		template <typename T> void RegisterSystem();

	  private:
		std::queue<EntityID> m_AvailableEntityIDs;
		std::array<Signature, MAX_ENTITIES> m_Signatures;
		u32 m_ActiveEntitiesCount;

		std::unordered_map<const char *, ComponentType> m_ComponentTypes{};
		std::unordered_map<const char *, IComponentArray *> m_ComponentArrays{};
		ComponentType m_NextComponentType{};

		std::unordered_map<const char *, System *> m_Systems{};
	};

} // namespace Object
} // namespace Cookie