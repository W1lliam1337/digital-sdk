#include "../hooks.hh"
#include "../../ent_listener/ent_listener.hh"

void c_hooks::client_ent_list_t::init( ) {
    static const auto on_add_entity_target = g_mem->get_virtual( g_interfaces->m_entity_list, 11 );
    g_modules->m_client_dll.hook_function( _( "CClientEntityList::OnAddEntity" ), &on_add_entity,
                                           on_add_entity_target );

    static const auto on_remove_entity_target = g_mem->get_virtual( g_interfaces->m_entity_list, 12 );
    g_modules->m_client_dll.hook_function( _( "CClientEntityList::OnRemoveEntity" ), &on_remove_entity,
                                           on_remove_entity_target );
}

void __fastcall c_hooks::client_ent_list_t::on_add_entity( REGS, c_handle_entity* entity, const c_base_handle handle ) {
    static const auto og = g_modules->m_client_dll.get<decltype( &on_add_entity )>( _( "CClientEntityList::OnAddEntity" ) );

    const auto unknown = reinterpret_cast<c_client_unknown* const>( entity );

    if ( const auto& base_entity = unknown->get_base_entity( ) )
        g_listener->add( base_entity );

    og( ecx, edx, entity, handle );
}

void __fastcall c_hooks::client_ent_list_t::on_remove_entity( REGS, c_handle_entity* entity, c_base_handle handle ) {
    static const auto og = g_modules->m_client_dll.get<decltype( &on_remove_entity )>( _( "CClientEntityList::OnRemoveEntity" ) );

    const auto unknown = reinterpret_cast<c_client_unknown* const>( entity );

    if ( const auto& base_entity = unknown->get_base_entity( ) )
        g_listener->remove( base_entity );

    og( ecx, edx, entity, handle );
}