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
#include "../../includes.h"

#include <d3d9.h>
#include <d3dx9.h>

using create_interface_fn = void*(*)(const char*, int*);

class c_interfaces
{
protected:
	void* get_interface(HMODULE module, const char* interface_name) const;
public:
	void init();

	i_global_vars_base* m_globals{};
	i_engine* m_engine{};
	i_client_entity_list* m_entity_list{};
	i_base_client_dll* m_client{};
	i_input* m_input{};
	i_surface* m_surface{};
	i_debug_overlay* m_debug_overlay{};
	IDirect3DDevice9* m_direct_device{};
	i_panel* m_panel{};
	i_engine_trace* m_trace{};
	i_cvar* m_cvar{};
	i_physics_surface_props* m_physics_surface_props{};
	i_move_helper* m_move_helper{};
	i_game_movement* m_game_movement{};
	i_prediction* m_prediction{};
	i_mdl_cache* m_mdl_cache{};
	i_model_info* m_model_info{};
	i_material_system* m_material_system{};
	i_studio_render* m_studio_render{};
	i_model_render* m_model_render{};
	i_render_view* m_render_view{};
	i_game_event_manager* m_event_manager{};
};

inline const auto g_interfaces = std::make_unique<c_interfaces>();