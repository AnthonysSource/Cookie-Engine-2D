#pragma once

#include "Components.h"
#include "CookieEngine.h"

#include "optick.h"

namespace Cookie {

	class RotateSystem : public System {
	public:
		void InitSignature() {
			m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
			m_Signature.set(g_Admin->GetComponentSignatureID<RotatingComponent>(), true);
		}

		void Update(f32 dt) override {
			OPTICK_EVENT();
			for (auto const &entityID : m_EntitiesCache) {
				TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
				RotatingComponent *f = g_Admin->GetComponent<RotatingComponent>(entityID);
				t->m_Rotation += Float3(0.0f, f->m_Speed, 0.0f) * dt;
			}
		}
	};

	class MoveSystem : public System {
	public:
		void InitSignature() {
			m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
			m_Signature.set(g_Admin->GetComponentSignatureID<MoveComponent>(), true);
		}

		void Update(f32 dt) override {
			OPTICK_EVENT();
			for (auto const &entityID : m_EntitiesCache) {
				TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
				MoveComponent *m = g_Admin->GetComponent<MoveComponent>(entityID);
				InputComponent *input = &InputSystem::g_InputComponent;

				if (input->IsKeyHeld(COOKIE_KEY_W)) {
					t->m_Position.y += m->m_Speed * dt;
				} else if (input->IsKeyHeld(COOKIE_KEY_S)) {
					t->m_Position.y -= m->m_Speed * dt;
				}

				if (input->IsKeyHeld(COOKIE_KEY_D)) {
					t->m_Position.x += m->m_Speed * dt;
				} else if (input->IsKeyHeld(COOKIE_KEY_A)) {
					t->m_Position.x -= m->m_Speed * dt;
				}
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
			OPTICK_EVENT();
			for (auto const &entityID : m_EntitiesCache) {
				TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
				FloatComponent *f = g_Admin->GetComponent<FloatComponent>(entityID);
				t->m_Position.y = f->m_Amplitude * cos(f->m_Speed * glfwGetTime());
			}
		}
	};

} // namespace Cookie