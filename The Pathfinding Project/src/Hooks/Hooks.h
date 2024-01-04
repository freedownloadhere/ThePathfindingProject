#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <iostream>

#include "MinHook.h"

namespace tpp::hooks
{
	typedef BOOL(WINAPI* type_wglSwapBuffers)(HDC);

	inline HWND window_handle{ nullptr };

	namespace original
	{
		inline WNDPROC WndProc{ nullptr };
		inline type_wglSwapBuffers wglSwapBuffers{ nullptr };
	}

	namespace hooked
	{
		extern LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
		extern BOOL WINAPI wglSwapBuffers(HDC);
	}

	bool init();
	bool destroy();
}