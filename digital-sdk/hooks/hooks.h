#pragma once
#include "../../includes.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"
#include "../../menu/menu.h"

#define HOOK(address, function, original) MH_CreateHook(address, function, reinterpret_cast<void**>(&(original))); printf("hooked %s\n", #function);\

using create_move_t = void(__stdcall*)(int, float, bool);
using present_t = long(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
using reset_t = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using lock_cursor_t = void(__thiscall*)(void*);
using paint_traverse_t = void(__thiscall*)(void*, vgui::vpanel, bool, bool);
using dme_t = void(__fastcall*)(void*, void*, void*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
using override_view_t = void(__thiscall*)(void*, c_view_setup*);
using modify_eye_position_t = void(__thiscall*)(void*, void*, c_vec3&);
using calculate_view_t = void(__fastcall*)(void*, void*, c_vec3&, qangle_t&, float&, float&, float&);
using registers_t = void(__fastcall*)(void*, void*);

class c_hooks
{
	static void __stdcall hk_create_move(int sequence, float frame_time, bool is_active, bool& send_packet);
	static void __stdcall hk_create_move_proxy(int sequence, float frame_time, bool is_active);
	static void init_wnd_proc();
	static long __stdcall hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override,
		const RGNDATA* dirty_region) noexcept;
	static long __stdcall hk_wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm);
	static long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
	static void __stdcall hk_lock_cursor() noexcept;
	static void __fastcall hk_paint_traverse(void* ecx, void* edx, vgui::vpanel panel, bool force_repaint, bool allow_force);
	static void __fastcall hk_modify_eye_position(void* ecx, void* edx, c_vec3& input_eye_pos);
	static void __fastcall hk_calculate_view(void* ecx, void* edx, c_vec3& eye_origin, qangle_t& eye_angles, float& z_near,
		float& z_far, float& fov);
	static void __fastcall hk_inferno_client_think(void* ecx, void* edx) noexcept;
	static void _cdecl hk_blood_spray_callback(void* ecx, void* edx) noexcept;
	static void __fastcall hk_draw_model_execute(void* ecx, void* edx, void* context, const draw_model_state_t& state,
		const model_render_info_t& info, matrix_t* custom_bone_to_world);
	static void __fastcall hk_override_view(void* ecx, void* edx, c_view_setup* setup_view);

	create_move_t m_og_create_move{};
	WNDPROC m_og_wnd_proc{};
	present_t m_og_present{};
	reset_t m_og_reset{};
	lock_cursor_t m_og_lock_cursor{};
	paint_traverse_t m_og_paint_traverse{};
	dme_t m_og_draw_model_execute{};
	override_view_t m_og_override_view{};
	modify_eye_position_t m_og_modify_eye_position{};
	calculate_view_t m_og_calculate_view{};
	registers_t m_registers{};

public:
	static void init();
};

inline const auto g_hooks = std::make_unique<c_hooks>();