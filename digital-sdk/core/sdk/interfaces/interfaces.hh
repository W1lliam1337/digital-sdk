#ifndef VALVE_INTERFACES
#define VALVE_INTERFACES

#include "classes/base_client.h"
#include "classes/client_mode.hh"
#include "classes/cvar.hh"
#include "classes/engine.hh"
#include "classes/engine_trace.hh"
#include "classes/engine_vgui.hh"
#include "classes/game_movement.hh"
#include "classes/globals.hh"
#include "classes/input.hh"
#include "classes/key_values_sys.hh"
#include "classes/mat_sys.hh"
#include "classes/model_cache.hh"
#include "classes/model_render.hh"
#include "classes/move_helper.hh"
#include "classes/phys_surface_props.hh"
#include "classes/prediction.h"
#include "classes/steam.hh"
#include "classes/surface.hh"

using create_interface_fn = void* ( * ) ( const char*, int* );
using instantiate_interface_fn = void* ( * ) ( );

class c_interface_reg {
public:
    c_interface_reg( instantiate_interface_fn fn, const char* name );
    instantiate_interface_fn m_create_fn{ };
    const char* m_name{ };
    c_interface_reg* m_next{ };
};

class c_interfaces final {
    template < typename T > T get( const c_module_info& the_module, std::string_view interface_name );
public:
	void init( );

    // TODO: create a struct for this or something like that - g_interfaces->client( )
    i_base_client_dll* m_client{ };
    i_client_entity_list* m_entity_list{ };
    i_move_helper* m_move_helper{ };
    i_game_movement* m_game_movement{ };
    i_engine_client* m_engine{ };
    i_global_vars* m_globals{ };
    i_engine_trace* m_trace{ };
    c_client_state* m_client_state{ };
    c_move_data* m_move_data{ };
    i_input* m_input{ };
    i_prediction* m_prediction{ };
    i_client_mode* m_client_mode{ };
    h_steam_user m_steam_user{ };
    h_steam_pipe m_steam_pipe{ };
    i_steam_client* m_steam_client{ };
    i_steam_screenshots* m_steam_screenshots{ };
    i_cvar* m_cvar{ };
    i_phys_surface_props* m_phys_surface_props{ };
    IDirect3DDevice9* m_device{ };
    i_surface* m_surface{ };
    c_engine_vgui* m_engine_vgui{ };
    i_material_system* m_material_system{ };
    c_key_values_system* m_key_values_system{ };
    i_model_render* m_model_render{ };
    i_model_cache* m_model_cache{ };
};

inline const std::unique_ptr< c_interfaces > g_interfaces{ new c_interfaces( ) };

#endif