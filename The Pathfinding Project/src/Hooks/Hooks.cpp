#include "Hooks.h"

using namespace tpp;

bool hooks::init()
{
	window_handle = FindWindowA("LWJGL", nullptr);
	if (window_handle == nullptr)
	{
		std::cerr << "[-] Failed to get the window handle.\n";
		return false;
	}

	original::WndProc = (WNDPROC)SetWindowLongPtr(window_handle, GWLP_WNDPROC, (LONG_PTR)detour::WndProc);
	if (original::WndProc == nullptr)
	{
		std::cerr << "[-] Failed to get the WndProc function.\n";
		return false;
	}

#pragma warning( suppress : 6387 )
	original::wglSwapBuffers = (type_wglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
	if (original::wglSwapBuffers == nullptr)
	{
		std::cerr << "[-] Failed to get the wglSwapBuffers process.\n";
		return false;
	}

	MH_Initialize();
	MH_CreateHook(original::wglSwapBuffers, &detour::wglSwapBuffers, (void**)&original::wglSwapBuffers);
	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

bool hooks::destroy()
{
	SetWindowLongPtr(window_handle, GWLP_WNDPROC, (LONG_PTR)original::WndProc);

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	return true;
}