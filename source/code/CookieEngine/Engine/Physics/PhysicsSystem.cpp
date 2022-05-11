#include "PhysicsSystem.h"
#include "Core/Application.h"

#include "Entities/Components/PhysicsComponent.h"

#include <box2d/box2d.h>

namespace Cookie {

	struct PhysicsSysState {
		b2World *m_World;
	};

	static PhysicsSysState g_physicsState{};

	void PhysicsSystem::Init() {

		// Create Physics World
		b2Vec2 gravity(0.0f, -10.0f);
		g_physicsState.m_World = new b2World(gravity);
	}

	void PhysicsSystem::InitSignature() { m_Signature.set(g_Admin->GetComponentSignatureID<PhysicsComponent>(), true); }

	void PhysicsSystem::Update(f32 dt) { g_physicsState.m_World->Step(dt, 8, 3); }

	void PhysicsSystem::Shutdown() {
		// Deallocate physics world
		delete g_physicsState.m_World;
	}

} // namespace Cookie
