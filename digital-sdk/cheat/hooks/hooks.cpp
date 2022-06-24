#include "hooks.h"
#include "../functions/includes.h"
#include "../menu/menu.h"

void c_hooks::init()
{
	if (MH_Initialize() != MH_OK)
		return;

	init_wnd_proc();

	const auto create_move = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_sdk.m_interfaces.m_client))[22]);
	const auto reset = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_sdk.m_interfaces.m_direct_device))[16]);
	const auto present = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_sdk.m_interfaces.m_direct_device))[17]);
	const auto lock_cursor = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_sdk.m_interfaces.m_surface))[67]);
	const auto paint_traverse = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_sdk.m_interfaces.m_panel))[41]);

	HOOK(create_move, hk_create_move_proxy, g_sdk.m_hooks_data.m_originals.m_create_move);
	HOOK(reset, hk_reset, g_sdk.m_hooks_data.m_originals.m_reset);
	HOOK(present, hk_present, g_sdk.m_hooks_data.m_originals.m_present);
	HOOK(lock_cursor, hk_lock_cursor, g_sdk.m_hooks_data.m_originals.m_lock_cursor);
	HOOK(paint_traverse, hk_paint_traverse, g_sdk.m_hooks_data.m_originals.m_paint_traverse);

	MH_EnableHook(nullptr);
}

void c_hooks::init_wnd_proc()
{
	D3DDEVICE_CREATION_PARAMETERS parameters;
	g_sdk.m_interfaces.m_direct_device->GetCreationParameters(&parameters);

	const auto wnd = parameters.hFocusWindow;
	if (!wnd)
		return;

	g_sdk.m_hooks_data.m_originals.m_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wnd_proc)));
}

long __stdcall c_hooks::hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region) noexcept
{
	if (!ImGui::GetCurrentContext())
		return g_sdk.m_hooks_data.m_originals.m_present(device, src, dest, window_override, dirty_region);

	ImGui_ImplDX9_Init(device);
	c_menu::get()->init_style();

	IDirect3DVertexDeclaration9* vert_declaration;
	IDirect3DVertexShader9* vert_shader;
	DWORD old_d3drs_colorwriteenable;

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &old_d3drs_colorwriteenable);
	device->GetVertexDeclaration(&vert_declaration);
	device->GetVertexShader(&vert_shader);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::IsKeyPressed(VK_INSERT, false))
		g_sdk.m_menu_data.m_is_menu_opened = !g_sdk.m_menu_data.m_is_menu_opened;

	c_menu::get()->render();

	ImGui::EndFrame();
	ImGui::Render(c_render::get()->render_scene());

	if (device->BeginScene() == D3D_OK)
	{
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	device->SetRenderState(D3DRS_COLORWRITEENABLE, old_d3drs_colorwriteenable);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vert_declaration);
	device->SetVertexShader(vert_shader);

	return g_sdk.m_hooks_data.m_originals.m_present(device, src, dest, window_override, dirty_region);
}

long __stdcall c_hooks::hk_wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) noexcept
{
	ImGui_ImplWin32_Init(window);

	c_utils::init_key_sys(msg, wparm);

	if (ImGui_ImplWin32_WndProcHandler(window, msg, wparm, lparm) && g_sdk.m_menu_data.m_is_menu_opened && !(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_MOUSEMOVE))
		return false;

	if (g_sdk.m_menu_data.m_is_menu_opened && (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_MOUSEMOVE))
		return false;

	return CallWindowProcA(g_sdk.m_hooks_data.m_originals.m_wnd_proc, window, msg, wparm, lparm);
}

long __stdcall c_hooks::hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto og = g_sdk.m_hooks_data.m_originals.m_reset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return og;
}

void __stdcall c_hooks::hk_lock_cursor() noexcept
{
	if (g_sdk.m_menu_data.m_is_menu_opened)
		return g_sdk.m_interfaces.m_surface->unlock_cursor();

	return g_sdk.m_hooks_data.m_originals.m_lock_cursor(g_sdk.m_interfaces.m_surface);
}

__declspec(naked) void __stdcall c_hooks::hk_create_move_proxy(int sequence, float frame_time, bool is_active)
{
	__asm
	{
		push ebp
		mov ebp, esp
		push ebx
		lea ecx, [esp]
		push ecx
		push dword ptr[ebp + 10h]
		push dword ptr[ebp + 0Ch]
		push dword ptr[ebp + 8]
		call hk_create_move
		pop ebx
		pop ebp
		retn 0Ch
	}
}

void __stdcall c_hooks::hk_create_move(int sequence, float frame_time, bool is_active, bool& send_packet)
{
	g_sdk.m_hooks_data.m_originals.m_create_move(sequence, frame_time, is_active);

	const auto cmd = g_sdk.m_interfaces.m_input->get_user_cmd(sequence);
	const auto verified_cmd = g_sdk.m_interfaces.m_input->get_verified_cmd(sequence);

	if (!cmd || !verified_cmd)
		return;

	g_sdk.m_packet_data.m_cmd = cmd;
	g_sdk.m_packet_data.m_send_packet = true /*frame_time == 0.0f*/;

	c_movement::get()->bunny_hop();
	c_engine_prediction::get()->setup();
	c_engine_prediction::get()->begin();
	{
		c_anti_aimbot::get()->init();
		//c_ragebot::get()->init();
	}
	c_engine_prediction::get()->end();

	c_utils::get()->fix_movement();

	send_packet = g_sdk.m_packet_data.m_send_packet;

	verified_cmd->m_cmd = *g_sdk.m_packet_data.m_cmd;
	verified_cmd->m_crc = g_sdk.m_packet_data.m_cmd->get_checksum();
}

void __fastcall c_hooks::hk_paint_traverse(void* ecx, void* edx, vgui::vpanel panel, bool force_repaint, bool allow_force)
{
	static uint32_t hud_zoom_panel = 0;

	if (!hud_zoom_panel)
		if (!strcmp(_("HudZoom"), g_sdk.m_interfaces.m_panel->get_name(panel)))
			hud_zoom_panel = panel;

	if (hud_zoom_panel == panel && g_cfg.m_misc.m_no_scope)
		return;

	g_sdk.m_hooks_data.m_originals.m_paint_traverse(ecx, panel, force_repaint, allow_force);

	static vgui::vpanel panel_id = 0;

	if (!panel_id)
	{
		if (!strcmp(g_sdk.m_interfaces.m_panel->get_name(panel), "MatSystemTopPanel"))
			panel_id = panel;
	}

	if (panel_id == panel)
	{
		c_render::get()->begin();
		{
			c_player_esp::get()->draw();
		}
		c_render::get()->end();
	}
}
