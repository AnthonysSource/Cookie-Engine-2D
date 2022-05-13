#pragma once

#include "Components.h"
#include "CookieEngine.h"

#include "optick.h"

namespace Cookie {

	class RotateSystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<RotatingComponent>();
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			auto transforms = g_Admin->GetComponentArray<TransformComponent>();
			auto rotations = g_Admin->GetComponentArray<RotatingComponent>();

			for (auto const &entityID : m_Entities) {
				TransformComponent *t = transforms->Get(entityID);
				RotatingComponent *f = rotations->Get(entityID);
				t->m_Rotation += Float3(0.0f, f->m_Speed, 0.0f) * dt;
			}
		}
	};

	class PlayerMovementSystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<PlayerCharacterComponent>();
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			auto transforms = g_Admin->GetComponentArray<TransformComponent>();
			auto players = g_Admin->GetComponentArray<PlayerCharacterComponent>();
			InputComponent *input = g_Admin->GetSinglComponent<InputComponent>();

			for (auto const &entityID : m_Entities) {

				TransformComponent *t = transforms->Get(entityID);
				PlayerCharacterComponent *m = players->Get(entityID);

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
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<FloatComponent>();
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			auto transforms = g_Admin->GetComponentArray<TransformComponent>();
			auto floats = g_Admin->GetComponentArray<FloatComponent>();

			for (auto const &entityID : m_Entities) {
				TransformComponent *t = transforms->Get(entityID);
				FloatComponent *f = floats->Get(entityID);
				t->m_Position.y = f->m_Amplitude * (float)cos(f->m_Speed * glfwGetTime());
			}
		}
	};

} // namespace Cookie