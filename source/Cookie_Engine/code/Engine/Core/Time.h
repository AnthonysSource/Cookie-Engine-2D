#pragma once

#include "Core/Types/PrimitiveTypes.h"

namespace Cookie {
	struct Time {

		static f64 m_SecondsUpTime;
		static f64 m_SecondsUpTimeLastUpdate;

		static u64 m_TicksUptime;
		static u64 m_TicksUptimeLastUpdate;

		static f32 m_DeltaTime;
	};
} // namespace Cookie