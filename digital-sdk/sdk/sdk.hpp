#pragma once

/* microsoft */
#include <vector>
#include <Windows.h>
#include <string>
#include <cmath>
#include <string_view>
#include <chrono>
#include <thread>
#include <array>

/* dependencies */
#include "../dependencies/singleton/singleton.h"
#include "../dependencies/vfunc/vfunc.h"
#include "../dependencies/obfuscation/xor.h"
#include "../dependencies/obfuscation/fnv1a.h"
#include "../dependencies/bit flags/big_flag.h"
#include "../dependencies/config manager/config_manager.h"
#include "../dependencies/color/color.h"
#include "../dependencies/render/render.h"
#include "utils/vertex/vertex.h"
#include "utils/data map/data_map.h"
#include "utils/recv table/recv.h"
#include "utils/crc32/checksum_crc.h"
#include "utils/bfread/bf_read.h"
#include "utils/studio/studio.h"
#include "math/math.h"
#include "math/qangle.h"
#include "math/vec3_t.h"
#include "math/vec2_t.h"
#include "math/vec4_t.h"
#include "math/vmatrix.h"
#include "utils/handle/base_handle.h"

/* interfaces */
#include "interfaces/classes/i_engine_trace.h"
#include "interfaces/classes/i_model_render.h"
#include "interfaces/classes/i_client_entity.h"
#include "interfaces/classes/i_client.h"
#include "interfaces/classes/i_globals.h"
#include "interfaces/classes/i_engine.h"
#include "interfaces/classes/i_input.h"
#include "interfaces/classes/c_user_cmd.h"
#include "interfaces/classes/i_client_entity_list.h"
#include "interfaces/classes/i_prediction.h"
#include "interfaces/classes/i_move_helper.h"
#include "interfaces/classes/i_game_movement.h"
#include "interfaces/classes/i_model_cache.h"
#include "interfaces/classes/c_move_data.h"
#include "interfaces/classes/i_weapon_system.h"
#include "interfaces/classes/i_surface.h"
#include "interfaces/classes/i_debug_overlay.h"
#include "interfaces/classes/i_panel.h"
#include "interfaces/classes/i_model_info.h"
#include "interfaces/classes/i_cvar.h"
#include "interfaces/classes/i_convar.h"
#include "interfaces/classes/i_prop_physics.h"
#include "interfaces/classes/i_engine_trace.h"
#include "interfaces/classes/i_multiplayer_physics.h"
#include "interfaces/classes/i_breakable_with_prop_data.h"
#include "interfaces/classes/i_event_manager.h"
#include "interfaces/classes/i_render_view.h"
#include "entity/enums.h"
#include "entity/classes.h"
#include "entity/entity.h"

/* graphics */
#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_internal.h"
#include "../dependencies/imgui/imgui_impl_dx9.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>

using create_move_t = void(__stdcall*)(int, float, bool);
using present_t = long (__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
using reset_t = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using lock_cursor_t = void(__thiscall*)(void*);
using paint_traverse_t = void(__thiscall*)(void*, vgui::vpanel, bool, bool);
using fsn_t = void(__thiscall*)(void*, int);
using override_view_t = void(__thiscall*)(void*, c_view_setup*);

class c_sdk
{
public:
	struct m_local_data_t
	{
		int m_screen_width{};
		int m_screen_height{};

		vec3_t m_shoot_pos{};
		vec3_t m_screen_size{};
	} m_local_data{};

	struct m_rage_data_t
	{
		c_base_player* m_player{};
	}m_rage_data{};

	struct m_player_data_t
	{
		bool m_anim_update{};
		bool m_setup_bones{};
	}m_player_data{};

	struct m_legit_data_t
	{
		
	}m_legit_data{};

	struct m_packet_data_t
	{
		c_user_cmd* m_cmd{};
		bool m_send_packet{};
	}m_packet_data{};

	struct fonts_t
	{
		ImFont* m_esp{};
		ImFont* m_logs{};
		std::string m_last_font_name;
	}m_fonts{};

	struct m_interfaces_t
	{
		i_base_client_dll* m_client{};
		i_engine* m_engine{};
		i_input* m_input{};
		i_global_vars_base* m_globals{};
		i_client_entity_list* m_entity_list{};
		i_prediction* m_prediction{};
		i_move_helper* m_move_helper{};
		i_game_movement* m_game_movement{};
		i_mdl_cache* m_model_cache{};
		i_weapon_system* m_weapon_system{};
		i_surface* m_surface{};
		IDirect3DDevice9* m_direct_device{};
		i_debug_overlay* m_debug_overlay{};
		i_panel* m_panel{};
		i_model_info* m_model_info{};
		i_cvar* m_cvar{};
		i_physics_surface_props* m_physics_surface_props{};
		c_engine_trace* m_trace{};
		i_game_event_manager* m_event_manager{};
	}m_interfaces{};

	struct m_module_list_t
	{
		HMODULE m_client_dll{};
		HMODULE m_engine_dll{};
		HMODULE m_vgui_dll{};
		HMODULE m_vgui2_dll{};
		HMODULE m_tier_dll{};
		HMODULE m_mat_sys_dll{};
		HMODULE m_localize_dll{};
		HMODULE m_shader_dll{};
		HMODULE m_data_cache_dll{};
		HMODULE m_vstd_dll{};
		HMODULE m_physics_dll{};
		HMODULE m_file_system_dll{};
		HMODULE m_server_dll{};
		HMODULE m_studio_render_dll{};
	}m_modules{};

	struct m_hooks
	{
		struct m_originals
		{
			create_move_t m_create_move{};
			present_t m_present{};
			reset_t m_reset{};
			lock_cursor_t m_lock_cursor{};
			WNDPROC m_wnd_proc{};
			paint_traverse_t m_paint_traverse{};
			fsn_t m_frame_stage_notify{};
			override_view_t m_override_view{};
		}m_originals{};
	}m_hooks_data{};

	struct menu_data_t
	{
		bool m_is_menu_opened{};
	}m_menu_data{};

	struct m_key_data_t
	{
		std::array<bool, 256> m_holded_keys{};
		std::array<bool, 256> m_toggled_keys{};
	}m_key_data;

	[[nodiscard]] c_base_player* m_local() const;
};

__forceinline c_base_player* c_sdk::m_local() const
{
	if (!m_interfaces.m_entity_list || !m_interfaces.m_engine)
		return nullptr;

	const auto local_player = reinterpret_cast<c_base_player*>(m_interfaces.m_entity_list->get_client_entity(m_interfaces.m_engine->get_local_player()));
	if (!local_player)
		return nullptr;

	return local_player;
}

#define TIME_TO_TICKS(time_) ((int)(0.5f + (float)((time_)) / g_sdk.m_interfaces.m_globals->m_interval_per_tick))
#define TICKS_TO_TIME(tick) (float)((tick) * g_sdk.m_interfaces.m_globals->m_interval_per_tick)

inline c_sdk g_sdk = c_sdk();