#pragma once

#include "Core/Types/String.h"
#include "Core/Types/PrimitiveTypes.h"

#include <GLFW/glfw3.h>

namespace Cookie {

	struct WindowData {
		GLFWwindow *m_Handle;
		u32 m_Height;
		u32 m_Width;
		String m_Title;
	};

	struct WindowDescription {
		u32 m_Height;
		u32 m_Width;
		String m_Title;
		bool m_IsFullScreen;
		u16 m_RefreshRate;
		String m_IconPath;
	};

} // namespace Cookie