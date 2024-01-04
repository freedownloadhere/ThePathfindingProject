#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include "../Hooks/Hooks.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#include "../Pathfinder/Pathfinder.h"

namespace tpp::gui
{
	void destroy();

	inline bool should_draw{ false };
	inline ImGuiContext* context;
}