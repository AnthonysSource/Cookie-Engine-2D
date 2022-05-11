#pragma once

#include "Core/Math/Math.h"
#include "Core/Types/PrimitiveTypes.h"

namespace Cookie {

	struct CameraComponent {
		Float3 m_Position;
		Float3 m_Forward;
		Float3 m_Up;
	};

} // namespace Cookie
