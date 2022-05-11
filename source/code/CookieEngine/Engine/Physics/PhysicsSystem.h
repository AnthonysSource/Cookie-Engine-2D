#pragma once

#include "Entities/EntityAdmin.h"

namespace Cookie {

	class PhysicsSystem : public System {
	public:
		void Init();
		void InitSignature() override;
		void Update(f32) override;
		void Shutdown();
	};

} // namespace Cookie