#pragma once

#include "Core/Application.h"
#include "Core/Types/PrimitiveTypes.h"

namespace Cookie {

namespace WindowManagement {

	void CreateAppWindow(Window *window, u32 width, u32 height, const char *title);
	void SetWindowSize(Window *w, u32 width, u32 height);

} // namespace WindowManagement

} // namespace Cookie
