#pragma once

#include "Core/Types/Function.h"
#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/String.h"

#include "Core/Time/Time.h"

#include "Entities/EntityAdmin.h"

#include "GLFW/glfw3.h"

namespace Cookie {

	struct WindowData {
		GLFWwindow *m_Handle;
		u32 m_Height;
		u32 m_Width;
		String m_Title;
	};

	struct AppData {
		WindowData m_Window;
	};

	extern AppData g_AppData;
	extern EntityAdmin *g_Admin;

	namespace Application {

		void Run(TFunction<void(EntityAdmin *const EntitiesAdmin)> CreateWorld);

	} // namespace Application
} // namespace Cookie