#include "GUI.h"

namespace
{
	HDC device_context{ nullptr };
	HGLRC old_context{ nullptr }, new_context{ nullptr };
	bool opengl_init{ false };
}

static void gui_init()
{
	old_context = wglGetCurrentContext();

	tpp::gui::context = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(tpp::hooks::window_handle);
	ImGui_ImplOpenGL3_Init();

	tpp::gui::should_draw = true;
}

static void gui_draw()
{
	if (!tpp::gui::should_draw) return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("The Pathfinding Project - FreeDownloadHere");
	{
		static int start[3]{ }, target[3]{ };
		static bool flags[1 << 3 + 1]{ }; // lazy man's hashmap

		if (ImGui::InputInt3("Starting Position", start))
		{
			tpp::pathfinder::state.start.x = (double)start[0];
			tpp::pathfinder::state.start.y = (double)start[1];
			tpp::pathfinder::state.start.z = (double)start[2];
		}
		if(ImGui::InputInt3("Target Position", target))
		{
			tpp::pathfinder::state.target.x = (double)target[0];
			tpp::pathfinder::state.target.y = (double)target[1];
			tpp::pathfinder::state.target.z = (double)target[2];
		}

		if (ImGui::Checkbox("Set Blocks?", &flags[tpp::makepathflags::SETBLOCK]))
			tpp::pathfinder::state.flags ^= tpp::makepathflags::SETBLOCK;
		if (ImGui::Checkbox("Safe mode?", &flags[tpp::makepathflags::SAFE]))
			tpp::pathfinder::state.flags ^= tpp::makepathflags::SAFE;
		if (ImGui::Checkbox("Use previous cache?", &flags[tpp::makepathflags::USEPREVCACHE]))
			tpp::pathfinder::state.flags ^= tpp::makepathflags::USEPREVCACHE;
		//Traverse eventually

		if (ImGui::Button("Run Pathfinder", { 100, 50 }))
			tpp::pathfinder::make_path(
				tpp::pathfinder::state.start,
				tpp::pathfinder::state.target,
				tpp::pathfinder::state.flags,
				tpp::pathfinder::state.block_to_set
			);
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void tpp::gui::destroy()
{
	should_draw = false;

	wglMakeCurrent(device_context, new_context);

	should_draw = false;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext(gui::context);
	SetWindowLongPtr(tpp::hooks::window_handle, GWLP_WNDPROC, (LONG_PTR)hooks::original::WndProc);

	wglMakeCurrent(device_context, new_context);
	wglDeleteContext(new_context);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT WINAPI tpp::hooks::hooked::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYDOWN && wParam == VK_NUMPAD1)
		tpp::gui::should_draw = !tpp::gui::should_draw;

	if (tpp::gui::should_draw)
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

	return CallWindowProc(original::WndProc, hwnd, msg, wParam, lParam);
}

BOOL WINAPI tpp::hooks::hooked::wglSwapBuffers(HDC dc)
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