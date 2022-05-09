#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/Containers.h"

#include "Common.h"

namespace Cookie {

	class System {
	public:
		virtual void InitSignature() = 0;
		virtual void Update(f32 dt) = 0;

	protected:
		Signature m_Signature{};
		TSet<EntityID> m_Entities{};

		template <typename T> void SetRequiredComponent() {
			m_Signature.set(g_Admin->GetComponentSignatureID<T>(), true);
		}

		friend class EntityAdmin;
	};

} // namespace Cookie
