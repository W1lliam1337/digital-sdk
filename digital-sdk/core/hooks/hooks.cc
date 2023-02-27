#include "hooks.hh"

void c_hooks::init( ) const {
    if ( MH_Initialize( ) != MH_OK )
        return;

    m_client.init( );
    m_engine.init( );
    m_client_ent_list.init( );
    m_direct_device.init( );
    m_surface.init( );
    m_model_render.init( );
}

void c_hooks::unhook( ) {
    g_modules->m_client_dll.unload_functions( );
    g_modules->m_engine_dll.unload_functions( );
    g_modules->m_shaderapidx9_dll.unload_functions( );
    g_modules->m_materialsystem_dll.unload_functions( );
    g_modules->m_vguimatsurface_dll.unload_functions( );
    MH_Uninitialize( );
}