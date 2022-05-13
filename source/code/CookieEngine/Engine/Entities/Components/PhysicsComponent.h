#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include <box2d/b2_body.h>

namespace Cookie {

	struct PhysicsComponent {
		b2Body *m_Body;
	};

} // namespace Cookie
