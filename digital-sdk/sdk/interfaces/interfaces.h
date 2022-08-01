#pragma once
#include "classes/i_input.h"
#include "classes/i_client.h"
#include "classes/i_globals.h"
#include "classes/i_client_entity_list.h"
#include "classes/i_engine.h"
#include "classes/i_surface.h"
#include "classes/i_debug_overlay.h"
#include "classes/i_panel.h"
#include "classes/i_engine_trace.h"
#include "classes/i_cvar.h"
#include "classes/i_convar.h"
#include "classes/i_prop_physics.h"
#include "classes/i_breakable_with_prop_data.h"
#include "classes/i_multiplayer_physics.h"
#include "classes/i_app_sys.h"
#include "classes/i_prediction.h"
#include "classes/i_game_movement.h"
#include "classes/i_move_helper.h"
#include "classes/i_model_cache.h"
#include "classes/i_model_info.h"
#include "classes/i_material.h"
#include "classes/i_render_view.h"
#include "classes/i_event_manager.h"
#include "classes/i_client_state.h"
#include "classes/i_mem_alloc.h"
#include "../../includes.h"

#include <d3d9.h>
#include <d3dx9.h>

using create_interface_fn = void*(*)(const char*, int*);

namespace interfaces
{
	void* get_interface(HMODULE module, const char* interface_name);
	void init();

	inline i_global_vars_base* m_globals{};
	inline i_engine* m_engine{};
	inline i_client_entity_list* m_entity_list{};
	inline i_base_client_dll* m_client{};
	inline i_input* m_input{};
	inline i_surface* m_surface{};
	inline i_debug_overlay* m_debug_overlay{};
	inline IDirect3DDevice9* m_direct_device{};
	inline i_panel* m_panel{};
	inline i_engine_trace* m_trace{};
	inline i_cvar* m_cvar{};
	inline i_physics_surface_props* m_physics_surface_props{};
	inline i_move_helper* m_move_helper{};
	inline i_game_movement* m_game_movement{};
	inline i_prediction* m_prediction{};
	inline i_mdl_cache* m_mdl_cache{};
	inline i_model_info* m_model_info{};
	inline i_material_system* m_material_system{};
	inline i_studio_render* m_studio_render{};
	inline i_model_render* m_model_render{};
	inline i_render_view* m_render_view{};
	inline i_game_event_manager* m_event_manager{};
	inline i_client_state* m_client_state{};
	inline i_mem_alloc* m_mem_alloc{};
}