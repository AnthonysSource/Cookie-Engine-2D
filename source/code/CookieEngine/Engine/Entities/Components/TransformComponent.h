#pragma once

#include "Core/Math/Math.h"
#include "Core/Types/PrimitiveTypes.h"

namespace Cookie {

	struct TransformComponent {
		Float3 m_Position;
		Float3 m_Rotation;
		Float3 m_Scale;
	};

} // namespace Cookie
