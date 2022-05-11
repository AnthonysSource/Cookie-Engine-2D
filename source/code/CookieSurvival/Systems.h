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
				SinglMainPlayerComponent *mainPlayer = g_Admin->GetSinglComponent<SinglMainPlayerComponent>();

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

				mainPlayer->m_Position = t->m_Position;
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

			auto mainPlayer = m_Admin->GetSinglComponent<SinglMainPlayerComponent>();

			for (auto const &entityID : m_Entities) {
				TransformComponent *t = m_Admin->GetComponent<TransformComponent>(entityID);
				EnemyComponent *m = m_Admin->GetComponent<EnemyComponent>(entityID);

				Float3 dir = mainPlayer->m_Position - t->m_Position;
				m->m_Velocity += glm::normalize(dir) * dt * m->m_Acceleration;
				t->m_Position += m->m_Velocity * dt;
			}
		}
	};

} // namespace Cookie