#pragma once

#include "Core/Application.h"
#include "Core/Types/PrimitiveTypes.h"

namespace Cookie::WindowManagement {

	void CreateAppWindow(WindowData *window, u32 width, u32 height, const char *title);
	void SetWindowSize(WindowData *w, u32 width, u32 height);

} // namespace Cookie::WindowManagement
