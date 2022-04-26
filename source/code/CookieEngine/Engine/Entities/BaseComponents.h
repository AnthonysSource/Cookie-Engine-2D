#pragma once

#include "Core/Math/Math.h"
#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/Function.h"

namespace Cookie {

	struct TransformComponent {
		Float3 m_Position;
		Float3 m_Rotation;
		Float3 m_Scale;
	};

	struct RenderComponent {
		u32 m_SpriteHandle;
	};

	struct RenderInstancedComponent {};

	struct CameraComponent {
		Float3 m_Position;
		Float3 m_Forward;
		Float3 m_Up;
	};

	struct ScriptComponent {
		TFunction<void> m_OnCreate;
		TFunction<void> m_OnUpdate;
		TFunction<void> m_OnDestroy;
	};

} // namespace Cookie
