#include "../hooks.h"

void hooks::init_wnd_proc()
{
	D3DDEVICE_CREATION_PARAMETERS parameters;
	interfaces::m_direct_device->GetCreationParameters(&parameters);

	const auto wnd = parameters.hFocusWindow;
	if (!wnd)
		return;

	og::m_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wnd_proc)));
}

long __stdcall hooks::hk_wnd_proc(const HWND window, const UINT msg, const WPARAM wparm, const LPARAM lparm)
{
	ImGui_ImplWin32_Init(window);

	utils::init_key_sys(msg, wparm);

	if (ImGui_ImplWin32_WndProcHandler(window, msg, wparm, lparm) && ctx::menu_data::m_is_menu_opened && !(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_MOUSEMOVE))
		return false;

	if (ctx::menu_data::m_is_menu_opened && (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_MOUSEMOVE))
		return false;

	return CallWindowProcA(og::m_wnd_proc, window, msg, wparm, lparm);
}