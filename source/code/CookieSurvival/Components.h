#pragma once

#include "CookieEngine.h"

namespace Cookie {

	struct PlayerCharacterComponent {
		f32 m_Speed;
	};

	struct EnemyComponent {
		f32 m_Acceleration;
		f32 m_TopSpeed;
		Float3 m_Velocity;
	};

	struct SinglMainPlayerComponent {
		Float3 m_Position;
	};

	struct AttackComponent {
		i32 m_AttackDamage;
		f32 m_Area;
		f32 m_CooldownElapsed;
		f32 m_CooldownTotal;
	};

} // namespace Cookie