#include "interfaces.hh"

template < typename T > T c_interfaces::get( const c_module_info& the_module, const std::string_view interface_name ) {
    static const auto fn_hash = fnv1a::hash( "CreateInterface" );
    const auto& create_interface = g_pe->export_fn( the_module.get_module( ), fn_hash );
    if ( !create_interface )
        return T( );

    const auto& create_interface_fn = c_address( create_interface ).offset( 0x4 ).relative( );
    if ( !create_interface_fn )
        return T( );

    const auto interface_reg = create_interface_fn.offset( 0x6 ).get< c_interface_reg* >( 2 );
    for ( auto interface_node = interface_reg; interface_node != nullptr; interface_node = interface_node->m_next ) {
        const std::string name = interface_node->m_name;
        if ( name.starts_with( interface_name ) && std::isdigit( name[ interface_name.size( ) ] ) ) {
            const auto& interface_address = interface_node->m_create_fn( );
            if ( !interface_address )
                return T( );

            log_print( "found interface {} -> {}", name, interface_address );
            return reinterpret_cast< T >( interface_address );
        }
    }
    return T( );
}

void c_interfaces::init( ) {
    m_client = get< i_base_client_dll* const >( static_cast< c_module_info >( 
        g_modules->m_client_dll ), _( "VClient" ) );
    m_entity_list = get< i_client_entity_list* const >( static_cast< c_module_info >( 
        g_modules->m_client_dll ),_( "VClientEntityList" ) );
    m_game_movement = get< i_game_movement* const >( static_cast< c_module_info >( 
        g_modules->m_client_dll ), _( "GameMovement" ) );
    m_prediction = get< i_prediction* const >( static_cast< c_module_info >( 
        g_modules->m_client_dll ), _( "VClientPrediction" ) );
    m_engine = get< i_engine_client* const >( static_cast< c_module_info >( 
        g_modules->m_engine_dll ), _( "VEngineClient" ) );
    m_trace = get< i_engine_trace* const >( static_cast< c_module_info >( 
        g_modules->m_engine_dll ),_( "EngineTraceClient" ) );
    m_cvar = get< i_cvar* >( static_cast< const c_module_info >( 
        g_modules->m_vstd_dll ), _( "VEngineCvar" ) );
    m_phys_surface_props = get< i_phys_surface_props* const >( static_cast< c_module_info >( 
        g_modules->m_vphysics_dll ), _( "VPhysicsSurfaceProps" ) );
    m_surface = get< i_surface* const >( static_cast< c_module_info >( 
        g_modules->m_vguimatsurface_dll ), _( "VGUI_Surface" ) );
    m_engine_vgui = get< c_engine_vgui* const >( static_cast< c_module_info >( 
        g_modules->m_engine_dll ), _( "VEngineVGui" ) );
    m_material_system = get< i_material_system* const >( static_cast< c_module_info >( 
        g_modules->m_materialsystem_dll ), _( "VMaterialSystem" ) );
    m_model_render = get< i_model_render* const >( static_cast< c_module_info >( 
        g_modules->m_engine_dll ), _( "VEngineModel" ) );
    m_model_cache = get< i_model_cache* const >( static_cast< c_module_info >( 
        g_modules->m_data_cahe_dll ), _( "MDLCache" ) );

    m_globals = g_mem->get_vfunc( m_client, 11 ).offset( 0xA ).get< i_global_vars* >( 2 );
    m_client_state = g_mem->get_vfunc( m_engine, 12 ).offset( 0x10 ).get< c_client_state* >( 2 );
    m_input = g_mem->get_vfunc( m_client, 16 ).at< i_input* >( 0x1 );
    m_client_mode = g_mem->get_vfunc( m_client, 10 ).offset( 0x5 ).get< i_client_mode* >( 2 );

    m_move_data = g_modules->m_client_dll.get_address( _( "g_pMoveData" ) ).offset( 0x2 ).get< c_move_data* >( 2 );
    m_move_helper = g_modules->m_client_dll.get_address( _( "g_pMoveHelper" ) ).offset( 0x2 ).get< i_move_helper* >( 2 );
    m_device = g_modules->m_shaderapidx9_dll.get_address( _( "g_pDirect3DDevice9" ) ).offset( 0x1 ).get< IDirect3DDevice9* >( 2 );

    m_steam_user = static_cast< h_steam_user >(
        g_pe->export_fn( g_modules->m_steam_api_dll.get_module( ), fnv1a::hash( "SteamAPI_GetHSteamUser" ) ) );
    m_steam_pipe = static_cast< h_steam_pipe >(
        g_pe->export_fn( g_modules->m_steam_api_dll.get_module( ), fnv1a::hash( "SteamAPI_GetHSteamPipe" ) ) );
    m_key_values_system = reinterpret_cast< c_key_values_system* >(
        g_pe->export_fn( g_modules->m_vstd_dll.get_module( ), fnv1a::hash( "KeyValuesSystem" ) ) );

    if ( auto ctx = m_engine->get_steam_api_context( ) ) {
        m_steam_client = ctx->m_steam_client;                 // SteamAPI_SteamUser_v021
        m_steam_screenshots = ctx->m_steam_steam_screenshots; // SteamAPI_SteamScreenshots_v003
    }
}