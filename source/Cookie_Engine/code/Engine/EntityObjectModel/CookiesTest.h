#pragma once

#include "Core/Common.h"
#include "Core/Logging/Logging.h"
#include "Core/Math.h"
#include "Core/Random/Random.h"

#include <stdlib.h>

namespace Cookie {
namespace Object {

	struct TransformComponent {
		vec3 m_Position;
		vec3 m_Rotation;
		vec3 m_Scale;
	};

	std::vector<TransformComponent> m_Transforms;

	const u32 ENTITIES_AMOUNT = 10;

	void GenerateCookies() {
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
} // namespace Object
} // namespace Cookie