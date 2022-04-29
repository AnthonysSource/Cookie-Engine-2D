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

	struct WindowDescription {
		u32 m_Height;
		u32 m_Width;
		String m_Title;
		bool m_IsFullScreen;
		u16 m_RefreshRate;
		String m_IconPath;
	};

	struct AppData {
		WindowData m_Window;
	};

	struct GameInitData {
		WindowDescription m_WindowDesc;
		TFunction<void(EntityAdmin *const EntitiesAdmin)> m_CreateWorldFunc;
		TFunction<void()> m_LoadResourcesFunc;
	};

	extern AppData g_AppData;
	extern EntityAdmin *g_Admin;

	namespace Application {

		void Run(GameInitData *gameInitData);

	} // namespace Application
} // namespace Cookie