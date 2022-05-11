#pragma once

#include "Entities/EntityAdmin.h"

namespace Cookie {

	class RenderingSystem : public System {
	public:
		void Init();
		void InitSignature() override;
		void Update(f32) override;
		void Shutdown();
	};

} // namespace Cookie