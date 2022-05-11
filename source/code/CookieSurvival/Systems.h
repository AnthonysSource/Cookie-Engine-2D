#pragma once

#include "Components.h"
#include "CookieEngine.h"

namespace Cookie {

	class PlayerMovementSystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<PlayerCharacterComponent>();
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			for (auto const &entityID : m_Entities) {
				TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
				PlayerCharacterComponent *m = g_Admin->GetComponent<PlayerCharacterComponent>(entityID);
				InputComponent *input = g_Admin->GetSinglComponent<InputComponent>();

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

	class EnemySystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<EnemyComponent>();
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			auto playerComponents = g_Admin->GetComponentArray<PlayerCharacterComponent>();
			u64 size = playerComponents->Count();
			for (size_t i = 0; i < size; i++) {
				playerComponents->At(i)->m_Speed += dt;
			}

			for (auto const &entityID : m_Entities) {
				TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
				EnemyComponent *m = g_Admin->GetComponent<EnemyComponent>(entityID);
			}
		}
	};

} // namespace Cookie