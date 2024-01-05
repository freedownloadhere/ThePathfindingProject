#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include "../Hooks/Hooks.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#include "../Utils/Flags/MakePathFlags.h"

namespace tpp::gui
{
	void destroy();

	inline int start[3]{ }, target[3]{ };
	inline bool player_pos_start{ false }, player_pos_target{ false };
	inline int flags{ 0 };
	inline bool run{ false };

	inline bool state_changed{ false };

	inline bool should_draw{ false };
	inline ImGuiContext* context;
}