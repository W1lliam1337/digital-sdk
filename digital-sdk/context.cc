#include "context.hh"
#include "features/chams/chams.hh"
#include "core/hooks/hooks.hh"
#include "core/render/render.hh"

DWORD c_ctx::dllmain_t::load( void* param ) {
    while ( !( g_ctx->m_hwnd = FindWindowA( _( "Valve001" ), nullptr ) ) )
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );

#if DEBUG_CONSOLE
    g_console->attach( );
#endif
    log_print( "hello user!" );

    if ( !g_pe->init( ) )
        throw std::runtime_error( "failed to init pe" );

    if ( !g_modules->init( ) )
        throw std::runtime_error( "failed to init modules" );

    g_interfaces->init( );
    g_net_vars->init( );
    g_chams->init_material( );
    g_hooks->set_wnd_proc( );
    g_render->init( );
    g_hooks->init( );

    return 0;
}

bool c_ctx::dllmain_t::create_thread( DWORD WINAPI function( void* ), void* parameter ) {
    const auto handle = CreateThread( nullptr, 0, function, parameter, 0, nullptr );

    if ( !handle )
        return false;

    CloseHandle( handle );
    return true;
}