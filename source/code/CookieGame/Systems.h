#pragma once

#include "Components.h"
#include "CookieEngine.h"

namespace Cookie::Application {

class RotateSystem : public System {
  public:
	void InitSignature() {
		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<RotatingComponent>(), true);
	}

	void Update(f32 dt) override {
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			RotatingComponent *f = g_Admin->GetComponent<RotatingComponent>(entityID);
			t->m_Rotation += Float3(0.0f, f->m_Speed, 0.0f) * dt;
		}
	}
};

class FloatSystem : public System {
  public:
	void InitSignature() {
		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<FloatComponent>(), true);
	}

	void Update(f32 dt) override {
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			FloatComponent *f = g_Admin->GetComponent<FloatComponent>(entityID);
			t->m_Position.y = f->m_Amplitude * cos(f->m_Speed * glfwGetTime());
		}
	}
};

} // namespace Cookie::Application