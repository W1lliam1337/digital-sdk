#include "hooks.h"

void c_hooks::init()
{
	if (MH_Initialize() != MH_OK)
		return;

	init_wnd_proc();

	const auto create_move = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_interfaces.m_client))[22]);
	const auto paint_traverse = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_interfaces.m_panel))[41]);
	const auto reset = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_interfaces.m_direct_device))[16]);
	const auto present = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_interfaces.m_direct_device))[17]);
	const auto lock_cursor = reinterpret_cast<void*>((*reinterpret_cast<uintptr_t**>(g_interfaces.m_surface))[67]);
	const auto override_view = static_cast<void*>(g_utils.find_sig(g_modules.m_client_dll, _("55 8B EC 83 E4 F8 8B 4D 04 83 EC 58")));
	const auto modify_eye_position = static_cast<void*>(g_utils.find_sig(g_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14 83 7F 60")));
	const auto calculate_view = static_cast<void*>(g_utils.find_sig(g_modules.m_client_dll, _("55 8B EC 83 EC 14 53 56 57 FF 75 18")));
	const auto inferno_client_think = static_cast<void*>(g_utils.find_sig(g_modules.m_client_dll, _("55 8B EC 83 EC 20 53 56 57 8B F9 8B 0D ? ? ? ? 8B 81 ? ? ? ? 89 45 EC")));
	const auto blood_callback = static_cast<void*>(g_utils.find_sig(g_modules.m_client_dll, _("55 8B EC 8B 4D 08 F3 0F 10 51 ? 8D 51 18")));
	
	HOOK(create_move, hk_create_move_proxy, g_hooks.m_originals.m_create_move);
	HOOK(reset, hk_reset, g_hooks.m_originals.m_reset);
	HOOK(present, hk_present, g_hooks.m_originals.m_present);
	HOOK(lock_cursor, hk_lock_cursor, g_hooks.m_originals.m_lock_cursor);
	HOOK(paint_traverse, hk_paint_traverse, g_hooks.m_originals.m_paint_traverse);
	HOOK(override_view, hk_override_view, g_hooks.m_originals.m_override_view);
	HOOK(modify_eye_position, hk_modify_eye_position, g_hooks.m_originals.m_modify_eye_position);
	HOOK(calculate_view, hk_calculate_view, g_hooks.m_originals.m_calculate_view);
	HOOK(inferno_client_think, hk_inferno_client_think, g_hooks.m_originals.m_inferno_client_think);
	HOOK(blood_callback, hk_blood_spray_callback, g_hooks.m_originals.m_inferno_client_think);

	MH_EnableHook(nullptr);
}

void __stdcall c_hooks::hk_create_move(const int sequence, const float frame_time, const bool is_active, bool& send_packet)
{
	g_hooks.m_originals.m_create_move(sequence, frame_time, is_active);

	const auto cmd = g_interfaces.m_input->get_user_cmd(sequence);
	const auto verified_cmd = g_interfaces.m_input->get_verified_cmd(sequence);

	if (!cmd || !verified_cmd)
		return;

	g_sdk.m_packet_data.m_cmd = cmd;
	g_sdk.m_packet_data.m_send_packet = true;

	g_movement.bunny_hop();

	g_engine_prediction.setup();
	g_engine_prediction.begin();
	{
		g_legit_bot.init();
	}
	g_engine_prediction.end();

	send_packet = g_sdk.m_packet_data.m_send_packet;
	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->get_checksum();
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

void c_hooks::init_wnd_proc()
{
	D3DDEVICE_CREATION_PARAMETERS parameters;
	g_interfaces.m_direct_device->GetCreationParameters(&parameters);

	const auto wnd = parameters.hFocusWindow;
	if (!wnd)
		return;

	g_hooks.m_originals.m_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk_wnd_proc)));
}

long __stdcall c_hooks::hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, const HWND window_override, const RGNDATA* dirty_region) noexcept
{
	if (!ImGui::GetCurrentContext())
		return g_hooks.m_originals.m_present(device, src, dest, window_override, dirty_region);

	ImGui_ImplDX9_Init(device);
	g_menu.init_style();

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

	g_menu.render();

	ImGui::EndFrame();
	ImGui::Render(g_render.render_scene());

	if (device->BeginScene() == D3D_OK)
	{
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	device->SetRenderState(D3DRS_COLORWRITEENABLE, old_d3drs_colorwriteenable);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vert_declaration);
	device->SetVertexShader(vert_shader);

	return g_hooks.m_originals.m_present(device, src, dest, window_override, dirty_region);
}

long __stdcall c_hooks::hk_wnd_proc(const HWND window, const UINT msg, const WPARAM wparm, const LPARAM lparm)
{
	ImGui_ImplWin32_Init(window);

	g_utils.init_key_sys(msg, wparm);

	if (ImGui_ImplWin32_WndProcHandler(window, msg, wparm, lparm) && g_sdk.m_menu_data.m_is_menu_opened && !(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_MOUSEMOVE))
		return false;

	if (g_sdk.m_menu_data.m_is_menu_opened && (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_MOUSEMOVE))
		return false;

	return CallWindowProcA(g_hooks.m_originals.m_wnd_proc, window, msg, wparm, lparm);
}

long __stdcall c_hooks::hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto og = g_hooks.m_originals.m_reset(device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return og;
}

void __stdcall c_hooks::hk_lock_cursor() noexcept
{
	return g_sdk.m_menu_data.m_is_menu_opened ? g_interfaces.m_surface->unlock_cursor() : g_hooks.m_originals.m_lock_cursor(g_interfaces.m_surface);
}

void __fastcall c_hooks::hk_paint_traverse(void* ecx, void* edx, const vgui::vpanel panel, const bool force_repaint, const bool allow_force)
{
	static uint32_t hud_zoom_panel = 0;

	if (!hud_zoom_panel)
		if (!strcmp(_("HudZoom"), g_interfaces.m_panel->get_name(panel)))
			hud_zoom_panel = panel;

	if (hud_zoom_panel == panel && g_cfg.m_misc.m_no_scope)
		return;

	g_hooks.m_originals.m_paint_traverse(ecx, panel, force_repaint, allow_force);

	static vgui::vpanel panel_id = 0;

	if (!panel_id)
	{
		if (!strcmp(g_interfaces.m_panel->get_name(panel), "MatSystemTopPanel"))
			panel_id = panel;
	}

	if (panel_id != panel)
		return;

	g_render.begin();
	{
		g_logs.instance();
		g_player_esp.draw();
	}
	g_render.end();
}

void __fastcall c_hooks::hk_override_view(void* ecx, void* edx, c_view_setup* setup_view)
{
	if (!g_sdk.m_local() || !g_sdk.m_local()->is_alive())
	{
		g_interfaces.m_input->m_camera_in_third_person = false;
		return;
	}

	if (!g_interfaces.m_input->m_camera_in_third_person && g_utils.is_bind_active(g_cfg.m_misc.m_third_person_bind))
		g_interfaces.m_input->m_camera_in_third_person = true;
	else
	{
		if (!g_utils.is_bind_active(g_cfg.m_misc.m_third_person_bind))
			g_interfaces.m_input->m_camera_in_third_person = false;
	}

	g_interfaces.m_input->m_camera_offset.z = static_cast<float>(g_cfg.m_misc.m_third_person_distance);
	return g_hooks.m_originals.m_override_view(ecx, setup_view);
}

void __fastcall c_hooks::hk_modify_eye_position(void* ecx, void* edx, vec3_t& input_eye_pos)
{
	const auto anim_state = static_cast<c_anim_state*>(ecx);
	if (!anim_state)
		return g_hooks.m_originals.m_modify_eye_position(ecx, edx, input_eye_pos);

	if (!anim_state->m_hit_ground && anim_state->m_duck_amount == 0.0f)
	{
		anim_state->m_smooth_height_valid = false;
		anim_state->m_camera_smooth_height = 0x7F7FFFFF;
		return;
	}

	using bone_lookup_fn = int(__thiscall*)(void*, const char*);
	static auto lookup_bone = reinterpret_cast<bone_lookup_fn>(c_utils::find_sig(g_modules.m_client_dll, _("55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 0F 84")));

	auto head_pos = anim_state->m_player->get_bone_cache()[lookup_bone(anim_state->m_player, _("head_0"))].at(3);
	head_pos.z += 1.7f;

	if (input_eye_pos.z > head_pos.z)
	{
		const auto lol = abs(input_eye_pos.z - head_pos.z);
		const auto v22 = (lol - 4.0) * 0.16666667;
		float v23;

		if (v22 >= 0.0)
			v23 = fminf(v22, 1.0);
		else
			v23 = 0.0;

		input_eye_pos.z = (head_pos.z - input_eye_pos.z) * (v23 * v23 * 3.0 - v23 * v23 * 2.0 * v23) +
			input_eye_pos.z;
	}
}

void __fastcall c_hooks::hk_calculate_view(void* ecx, void* edx, vec3_t& eye_origin, qangle_t& eye_angles, float& z_near, float& z_far, float& fov)
{
	/* @note: fixing weird landing animation
	 * @ref eng: https://www.unknowncheats.me/forum/counterstrike-global-offensive/504841-fixing-weird-landing-animation.html
	 * @ref ru: https://yougame.biz/threads/258573/
	*/
	const auto player = static_cast<c_base_player*>(ecx);

	if (!player || !g_sdk.m_local() || player != g_sdk.m_local())
		return g_hooks.m_originals.m_calculate_view(ecx, edx, eye_origin, eye_angles, z_near, z_far, fov);

	const auto backup_use_new_anim_state = player->should_use_new_anim_state();

	player->should_use_new_anim_state() = false;
	{
		g_hooks.m_originals.m_calculate_view(ecx, edx, eye_origin, eye_angles, z_near, z_far, fov);
	}
	player->should_use_new_anim_state() = backup_use_new_anim_state;
}

/* @ ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/Effects/clientinferno.cpp#L89 */
void __fastcall c_hooks::hk_inferno_client_think(void* ecx, void* edx)
{
	if (!g_cfg.m_misc.m_remove_molotov)
		return g_hooks.m_originals.m_inferno_client_think(ecx, edx);

	return;
}

/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/fx_cs_blood.cpp#L376 */
/* @note: way of removing blood on hit */
void __cdecl c_hooks::hk_blood_spray_callback(void* ecx, void* edx)
{
	return;
}