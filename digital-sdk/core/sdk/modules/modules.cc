#include "modules.hh"
#include "../../../context.hh"

bool c_modules::init( ) {
    if ( !get( ) || !get_addresses( ) )
        return false;

    log_print( _( "modules and addresses passed" ) );
    return true;
}

bool c_modules::get( ) {
    m_client_dll = g_pe->get( _( "client.dll" ) );
    m_gameoverlayrenderer_dll = g_pe->get( _( "gameoverlayrenderer.dll" ) );
    m_engine_dll = g_pe->get( _( "engine.dll" ) );
    m_steam_api_dll = g_pe->get( _( "steam_api.dll" ) );
    m_vstd_dll = g_pe->get( _( "vstdlib.dll" ) );
    m_vguimatsurface_dll = g_pe->get( _( "vguimatsurface.dll" ) );
    m_data_cahe_dll = g_pe->get( _( "datacache.dll" ) );
    m_materialsystem_dll = g_pe->get( _( "materialsystem.dll" ) );
    m_vphysics_dll = g_pe->get( _( "vphysics.dll" ) );
    m_shaderapidx9_dll = g_pe->get( _( "shaderapidx9.dll" ) );

    return m_client_dll.get_module( ) && m_gameoverlayrenderer_dll.get_module( )
        && m_engine_dll.get_module( ) && m_steam_api_dll.get_module( ) && m_vstd_dll.get_module( )
        && m_vguimatsurface_dll.get_module( ) && m_data_cahe_dll.get_module( ) && m_materialsystem_dll.get_module( )
        && m_vphysics_dll.get_module( ) && m_shaderapidx9_dll.get_module( );
}

bool c_modules::get_addresses( ) {
    m_client_dll.add_address( _( "g_pInput" ), _( "C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 59 C3 CC CC CC CC CC 83 EC 10" ) );
    m_client_dll.add_address( _( "g_pMoveData" ), _( "FF 35 ? ? ? ? 56 85 DB" ) );
    m_client_dll.add_address( _( "g_pMoveHelper" ), _( "8B 0D ? ? ? ? 8B 46 08 68" ) );
    m_client_dll.add_address( _( "g_pClientState" ), _( "8B 0D ? ? ? ? 8B 46 08 68" ) );
    m_client_dll.add_address( _( "C_BasePlayer::GetLocalPlayer" ), _( "8B 0D ? ? ? ? 85 C9 0F 84 ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B 4D FC" ) );
    m_client_dll.add_address( _( "CUserCmd::CheckSum" ), _( "53 8B D9 83 C8" ) );
    m_client_dll.add_address( _( "CPrediction::RestoreEntityToPredFrame" ), _( "E8 ? ? ? ? FF 74 1E 1C" ), true );
    m_client_dll.add_address( _( "MD5_PseudoRandom" ), _( "55 8B EC 83 E4 F8 83 EC 70 6A" ) );
    m_client_dll.add_address( _( "C_BaseEntity::PredictionRandomSeed" ), _( "0F 5B C0 89 35" ) );
    m_client_dll.add_address( _( "C_BaseEntity::PredictionPlayer" ), _( "8B 47 40 A3" ) );
    m_client_dll.add_address(_( "C_BaseEntity::PhysicsRunThink" ), _( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );
    m_client_dll.add_address( _( "C_BaseEntity::UnkThinkFunc" ), _( "E8 ? ? ? ? EB 11 8B 86 ? ? ? ?" ), true );
    m_client_dll.add_address( _( "C_BaseEntity::PostThinkVPhysics" ), _( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) );
    m_client_dll.add_address( _( "C_BaseEntity::SimulatePlayerSimulatedEntities" ), _( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) );
    m_client_dll.add_address( _( "TraceFilter" ), _( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) );
    m_client_dll.add_address( _( "TraceFilterSkipEntities" ), _( "E8 ? ? ? ? F3 0F 10 94 24 ? ? ? ? F3 0F 5C" ), true );
    m_client_dll.add_address( _( "TraceFilterSkipTwoEntities" ), _( "C7 45 ? ? ? ? ? 89 45 E4 8B 01" ) );
    m_client_dll.add_address( _( "ClipTraceToPlayers" ), _( "E8 ? ? ? ? 83 C4 18 8A 56 37" ), true );
    m_client_dll.add_address( _( "C_BaseEntity::IsBreakableEntity" ), _( "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" ) );
    m_client_dll.add_address( _( "CEngineClient::ScreenMatrix" ), _( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) );
    m_client_dll.add_address( _( "CKeyValues::LoadFromBuffer" ), _( "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" ) );
    m_client_dll.add_address( _( "CKeyValues::Init" ), _( "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81" ) );
    m_client_dll.add_address( _( "C_BasePlayer::GetWeaponShootPos" ), _( "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?" ) );
	m_engine_dll.add_address( _( "CL_Move" ), _( "55 8B EC 81 EC 64 01" ) );
    m_engine_dll.add_address( _( "CL_Move::SendPacket" ), _( "B3 01 8B 01 8B 40 10 FF D0 84 C0 74 0F 80 BF ? ? ? ? ? 0F 84" ) );
    m_engine_dll.add_address( _( "CEngineVGui::Paint" ), _( "55 8B EC 83 EC 40 53 8B D9 8B 0D ? ? ? ? 89 5D F8" ) );
    m_gameoverlayrenderer_dll.add_address( _( "GameOverlayRenderer::HookFunc" ), _( "55 8B EC 51 8B 45 10 C7" ) );
    m_shaderapidx9_dll.add_address( _( "g_pDirect3DDevice9" ), _( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) );
    m_vguimatsurface_dll.add_address( _( "CMatSystemSurface::StartDrawing" ), _( "55 8B EC 83 E4 C0 83 EC 38" ) );
    m_vguimatsurface_dll.add_address( _( "CMatSystemSurface::FinishDrawing" ), _( "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?" ) );
    m_vguimatsurface_dll.add_address( _( "CMatSystemSurface::LockCursor" ), _( "80 3D ? ? ? ? ? 8B 91 ? ? ? ? 8B 0D ? ? ? ? C6 05 ? ? ? ? ? 8B 01 74 0B FF 35 ? ? ? ? 52 FF 50 34 C3 6A 00 52 FF 50 34 C3 55" ) );

    return m_client_dll.valid_addresses( ) && m_gameoverlayrenderer_dll.valid_addresses( )
        && m_engine_dll.valid_addresses( ) && m_shaderapidx9_dll.valid_addresses( )
        && m_vguimatsurface_dll.valid_addresses( );
}

bool c_loaded_module::add_address( const std::string_view name, const std::string_view pattern, const bool relative ) {
    auto address = find( pattern, name );
    if ( !address )
        return false;

    if ( relative ) {
        address = address.relative( );
        if ( !address )
            return false;
    }

    m_addresses.emplace( fnv1a::hash( name ), address );
    return true;
}

bool c_loaded_module::hook_function( const std::string_view name, const LPVOID custom_function, const LPVOID target ) {
    LPVOID function = !target ? m_addresses[ fnv1a::hash( name ) ].as< LPVOID >( ) : target;
    if ( !function )
        return false;

    if ( MH_CreateHook( function, custom_function, &m_originals[ fnv1a::hash( name ) ] ) == MH_OK
         && MH_EnableHook( function ) == MH_OK ) {
        log_print( "hooked {} -> {}", name.data( ), function );
    }

    return true;
}

void c_loaded_module::unload_functions( ) {
    if ( m_addresses.empty( ) ) {}

     std::ranges::for_each( m_addresses,
    					   [ ] ( const auto& entry ) { MH_DisableHook( entry.second ); } );
}

bool c_loaded_module::valid_addresses( ) {
    return std::ranges::all_of( m_addresses | std::views::values,
                                []( const c_address& address ) { return address.is_valid( ); } );
}