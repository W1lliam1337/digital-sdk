#include "../interfaces/interfaces.hh"

c_handle_entity* c_base_handle::get( ) const {
    return static_cast< c_handle_entity* const >( g_interfaces->m_entity_list->get_client_entity_from_handle( *this ) );
}