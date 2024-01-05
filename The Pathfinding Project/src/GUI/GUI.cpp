#include "GUI.h"

using namespace tpp;

namespace
{
	HDC device_context{ nullptr };
	HGLRC old_context{ nullptr }, new_context{ nullptr };
	bool opengl_init{ false };

	bool flag_cache[3]{ };
}

static void gui_helpbox(const char* contents)
{
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.f);
		ImGui::TextUnformatted(contents);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void gui_init()
{
	old_context = wglGetCurrentContext();

	gui::imgui_context = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hooks::window_handle);
	ImGui_ImplOpenGL3_Init();

	gui::should_draw = true;
}

static void gui_draw()
{
	if (!gui::should_draw) return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("The Pathfinding Project - FreeDownloadHere");
	{
		gui::pathfinder_state_changed |= ImGui::InputInt3("Start ", gui::start);
		ImGui::SameLine();
		if (ImGui::Button("Your Position 1", { 100, 25 }))
			gui::player_pos_start = true, gui::pathfinder_state_changed = true;

		gui::pathfinder_state_changed |= ImGui::InputInt3("Target", gui::target);
		ImGui::SameLine();
		if (ImGui::Button("Your Position 2", { 100, 25 }))
			gui::player_pos_target = true, gui::pathfinder_state_changed = true;

		if (ImGui::Checkbox("Set Blocks?", &flag_cache[0]))
			gui::flags ^= makepathflags::SETBLOCK, gui::pathfinder_state_changed = true;
		gui_helpbox(
			"Whether blocks will be placed to show the found path.\n"
			"Should only be used for single-player testing.\n"
			"This is currently the only way to see the path."
		);

		if (ImGui::Checkbox("Safe Mode?", &flag_cache[1]))
			gui::flags ^= makepathflags::SAFE, gui::pathfinder_state_changed = true;
		gui_helpbox(
			"Whether the pathfinder will try to find the first\n"
			"solid block below the start & target positions.\n"
			"Can fix some annoying fails and bugs."
		);

		if (ImGui::Checkbox("Use Previous Cache?", &flag_cache[2]))
			gui::flags ^= makepathflags::USEPREVCACHE, gui::pathfinder_state_changed = true;
		gui_helpbox(
			"If checked, the previous cache of walkable blocks\n"
			"will not be wiped. Can significantly improve performance\n"
			"and should be used if blocks don't change."
		);

		if (ImGui::Button("Run Pathfinder", { 120, 50 }))
			gui::run = true, gui::pathfinder_state_changed = true;
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui::destroy()
{
	should_draw = false;

	wglMakeCurrent(device_context, new_context);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext(gui::imgui_context);
	SetWindowLongPtr(hooks::window_handle, GWLP_WNDPROC, (LONG_PTR)hooks::original::WndProc);

	wglMakeCurrent(device_context, new_context);
	wglDeleteContext(new_context);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WINAPI hooks::detour::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYDOWN && wParam == VK_NUMPAD1)
		gui::should_draw = !gui::should_draw, gui::gui_state_changed = true;

	if (gui::should_draw)
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

	return CallWindowProc(original::WndProc, hwnd, msg, wParam, lParam);
}

BOOL WINAPI hooks::detour::wglSwapBuffers(HDC dc)
{
	if (!opengl_init)
	{
		device_context = dc;

		old_context = wglGetCurrentContext();

		if (new_context) wglDeleteContext(new_context);
		new_context = wglCreateContext(dc);

		gui_init();

		opengl_init = true;
	}

	wglMakeCurrent(dc, new_context);

	gui_draw();

	wglMakeCurrent(dc, old_context);

	return original::wglSwapBuffers(dc);
}