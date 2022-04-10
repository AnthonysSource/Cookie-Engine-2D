#pragma once

#include "EntityObjectModel/EntityAdmin.h"

namespace Cookie {

class RenderingSystem : public System {
  public:
	void Init();
	void Update(f32) override;
	void Shutdown();
};

} // namespace Cookie