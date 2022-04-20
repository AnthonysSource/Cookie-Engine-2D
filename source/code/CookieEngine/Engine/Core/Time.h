#pragma once

#include "Core/Types/PrimitiveTypes.h"

namespace Cookie {

	struct TimeData {
		f64 m_SecondsUpTime;
		f64 m_SecondsUpTimeLastUpdate;

		u64 m_TicksUptime;
		u64 m_TicksUptimeLastUpdate;

		f32 m_DeltaTime;
	};

} // namespace Cookie