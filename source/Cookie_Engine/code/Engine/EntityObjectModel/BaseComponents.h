#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include "Core/Math.h"

namespace Cookie {

struct TransformComponent {
	Float3 m_Position;
	Float3 m_Rotation;
	Float3 m_Scale;
};

struct RenderComponent {
	u32 m_SpriteID;
};

struct CameraComponent {
	Float3 m_Position;
	Float3 m_Forward;
	Float3 m_Up;
};

struct ScriptComponent {};

} // namespace Cookie
