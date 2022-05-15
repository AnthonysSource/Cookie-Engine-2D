#pragma once

#include "Components.h"
#include "CookieEngine.h"

namespace Cookie {

	class PlayerMovementSystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<PlayerCharacterComponent>();

			m_View = g_Admin->CreateView(m_Signature);
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			for (auto const &entityID : m_View->m_Entities) {
				TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
				PlayerCharacterComponent *m = g_Admin->GetComponent<PlayerCharacterComponent>(entityID);

				InputComponent *input = g_Admin->GetSinglComponent<InputComponent>();
				SinglMainPlayerComponent *mainPlayer = g_Admin->GetSinglComponent<SinglMainPlayerComponent>();

				Float3 movementDir = Float3(0.0f, 0.0f, 0.0f);

				if (input->IsKeyHeld(COOKIE_KEY_W)) {
					movementDir.y = 1.0f;
				} else if (input->IsKeyHeld(COOKIE_KEY_S)) {
					movementDir.y = -1.0f;
				}

				if (input->IsKeyHeld(COOKIE_KEY_D)) {
					movementDir.x = 1.0f;
				} else if (input->IsKeyHeld(COOKIE_KEY_A)) {
					movementDir.x = -1.0f;
				}

				if (movementDir.x > 0.01f || movementDir.x < -0.01f || movementDir.y > 0.01f || movementDir.y < -0.01f)
					movementDir = glm::normalize(movementDir);

				movementDir.z = 0.0f;

				// CKE_LOG_INFO(Log::Channel::Game, "%f / %f / %f", movementDir.x, movementDir.y, movementDir.z);

				t->m_Position += movementDir * m->m_Speed * dt;

				// Copy position into singleton main player component
				mainPlayer->m_Position = t->m_Position;
			}
		}

	private:
		EntitiesView *m_View;
	};

	class CameraSystem : public System {
	public:
		void InitSignature() {}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			CameraComponentSingl *camSingl = g_Admin->GetSinglComponent<CameraComponentSingl>();
			CameraComponent *cam = g_Admin->GetComponent<CameraComponent>(camSingl->m_MainCam);
			InputComponent *input = g_Admin->GetSinglComponent<InputComponent>();

			Float3 movementDir = Float3(0.0f, 0.0f, 0.0f);

			if (input->IsKeyHeld(COOKIE_KEY_I)) {
				movementDir.y = 1.0f;
			} else if (input->IsKeyHeld(COOKIE_KEY_K)) {
				movementDir.y = -1.0f;
			}

			if (input->IsKeyHeld(COOKIE_KEY_L)) {
				movementDir.x = 1.0f;
			} else if (input->IsKeyHeld(COOKIE_KEY_J)) {
				movementDir.x = -1.0f;
			}

			if (movementDir.x > 0.01f || movementDir.x < -0.01f || movementDir.y > 0.01f || movementDir.y < -0.01f)
				movementDir = glm::normalize(movementDir);

			movementDir.z = 0.0f;

			if (input->IsKeyHeld(COOKIE_KEY_U)) {
				movementDir.z += 1.0f;
			} else if (input->IsKeyHeld(COOKIE_KEY_O)) {
				movementDir.z -= 1.0f;
			}

			cam->m_Position += movementDir * 5.0f * dt;
		}
	};

	class AttackSystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<AttackComponent>();
			m_AttackersView = g_Admin->CreateView(m_Signature);

			m_Signature = Signature();
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<EnemyComponent>();
			m_TargetsView = g_Admin->CreateView(m_Signature);
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			auto targetTransforms = g_Admin->GetComponentArray<TransformComponent>();
			auto attackers = g_Admin->GetComponentArray<AttackComponent>();

			for (auto const &attackerID : m_AttackersView->m_Entities) {
				TransformComponent *attackerTransform = targetTransforms->Get(attackerID);
				AttackComponent *attackerAttack = attackers->Get(attackerID);

				InputComponent *input = g_Admin->GetSinglComponent<InputComponent>();

				attackerAttack->m_CooldownElapsed += dt;

				if (input->IsKeyHeld(COOKIE_KEY_SPACE) && attackerAttack->m_CooldownElapsed > attackerAttack->m_CooldownTotal) {
					attackerAttack->m_CooldownElapsed = 0.0f;
					// TODO: Revisit when a Spatial-Hierarchy has been implemented to avoid
					//
					for (auto const &targetID : m_TargetsView->m_Entities) {
						TransformComponent *targetTransf = targetTransforms->Get(targetID);
						if (glm::length(targetTransf->m_Position - attackerTransform->m_Position) < attackerAttack->m_Area) {
							targetTransf->m_Position += Float3(15.0f, 0.0f, 0.0f);
						}
					}
				}
			}
		}

	private:
		EntitiesView *m_AttackersView;
		EntitiesView *m_TargetsView;
	};

	class EnemySystem : public System {
	public:
		void InitSignature() {
			SetRequiredComponent<TransformComponent>();
			SetRequiredComponent<EnemyComponent>();

			m_View = g_Admin->CreateView(m_Signature);
		}

		void Update(f32 dt) override {
			CKE_PROFILE_EVENT();

			auto mainPlayer = m_Admin->GetSinglComponent<SinglMainPlayerComponent>();
			auto transforms = m_Admin->GetComponentArray<TransformComponent>();
			auto enemies = m_Admin->GetComponentArray<EnemyComponent>();

			for (auto const &entityID : m_View->m_Entities) {
				TransformComponent *t = transforms->Get(entityID);
				EnemyComponent *m = enemies->Get(entityID);

				Float3 dir = mainPlayer->m_Position - t->m_Position;
				dir.z = 0.0f;
				f32 distance = glm::length(dir);
				m->m_Velocity += dir * (dt * m->m_Acceleration / distance);

				// Top Speed
				if (glm::length(m->m_Velocity) > m->m_TopSpeed) {
					m->m_Velocity = glm::normalize(m->m_Velocity) * m->m_TopSpeed;
				}

				t->m_Position += m->m_Velocity * dt;

				// Move out of screen if too close
				// if (distance > 0.3f) {
				//		t->m_Position += m->m_Velocity * dt;
				//} else {
				//	t->m_Position += Float3(10.0f, 0.0f, 0.0f);
				//}
			}
		}

	private:
		EntitiesView *m_View;
	};

} // namespace Cookie