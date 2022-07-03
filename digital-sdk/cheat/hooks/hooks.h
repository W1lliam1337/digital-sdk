#pragma once
#include <MinHook.h>
#pragma comment( lib, "MinHook.lib" )
#include <cstdint>

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_internal.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "../../dependencies/singleton/singleton.h"

#define HOOK(Address, Function, Original) MH_CreateHook((void*)(Address), (void*)(Function), reinterpret_cast<void**>(&(Original)));

struct qangle_t;
class c_view_setup;
class i_keyvalues_system;
class matrix_t;
class vec3_t;
using quaternion = float[4];

class c_hooks : public c_singleton<c_hooks>
{
	static void __stdcall hk_create_move_proxy(int sequence, float frame_time, bool is_active);
	static void __stdcall hk_create_move(int sequence, float frame_time, bool is_active, bool& send_packet);
	static long __stdcall hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override,
	                                 const RGNDATA* dirty_region) noexcept;

	static long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
	static long __stdcall hk_wnd_proc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) noexcept;
	static void __stdcall hk_lock_cursor() noexcept;
	static void __fastcall hk_paint_traverse(void* ecx, void* edx, unsigned panel, bool force_repaint, bool allow_force);
	static void __fastcall hk_frame_stage_notify(void* ecx, void* edx, int stage);
	static void __fastcall hk_override_view(void* ecx, void* edx, c_view_setup* setup_view);
	static void __fastcall hk_modify_eye_position(void* ecx, void* edx, vec3_t& input_eye_pos);
	static void __fastcall hk_calculate_view(void* ecx, void* edx, vec3_t& eye_origin, qangle_t& eye_angles, float& z_near, float& z_far, float& fov);
	static void* __fastcall	hk_alloc_keyvalues_memory(i_keyvalues_system* thisptr, int edx, int iSize);
public:
	static void init();
	static void init_wnd_proc();
};

// move this lol
__forceinline std::uintptr_t get_absolute_address(const std::uintptr_t uRelativeAddress)
{
	return uRelativeAddress + 0x4 + *reinterpret_cast<std::int32_t*>(uRelativeAddress);
}
