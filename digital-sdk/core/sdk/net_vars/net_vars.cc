#include "net_vars.hh"
#include "../interfaces/interfaces.hh"

// @ref: ClientDLL_InitRecvTableMgr()
void c_net_vars::init( ) {
    for ( auto cur = g_interfaces->m_client->get_all_classes( ); cur; cur = cur->m_next ) {
        if ( !cur->m_recv_table )
            continue;

        add_table( cur->m_recv_table->m_net_table_name, cur->m_recv_table );
    }
}

void c_net_vars::add_table( std::string_view name, const recv_table_t* recv_table, const std::size_t offset ) {
    m_offsets[ fnv1a::hash( recv_table->m_net_table_name ) ] = 0;

    for ( int i{ }; i < recv_table->m_props; ++i ) {
        const auto* prop = &recv_table->m_props_pointer[ i ];
        const auto* child = prop->m_data_table;

        if ( child && child->m_props > 0 ) {
            add_table( std::string( name.data( ) ) + "->" + prop->m_var_name, child, offset + prop->m_offset );
        }

        if ( prop->m_recv_type == dpt_datatable || std::strcmp( prop->m_var_name, "baseclass" ) == 0 )
            continue;

        const auto& netvar_name = std::string( name ) + "->" + prop->m_var_name;
        if ( !m_offsets.contains( fnv1a::hash( netvar_name ) ) ) {
            m_offsets[ fnv1a::hash( netvar_name ) ] = prop->m_offset + offset;
        }
    }
}