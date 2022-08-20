#include "netvar_manager.h"
#include <ostream>

std::vector<net_var_table_t> m_database;

void net_vars::init( ) {
	m_database.clear( );

	for ( auto client_class = interfaces::m_client->get_all_classes( );
		  client_class;
		  client_class = client_class->m_next ) {
		if ( !client_class->m_recv_table )
			continue;

		m_database.emplace_back( load_table( client_class->m_recv_table ) );
	}
}

net_var_table_t net_vars::load_table( const recv_table_t* recv_table ) {
	auto table = net_var_table_t{};

	table.m_offset = 0;
	table.m_name = recv_table->m_net_table_name;

	for ( auto i = 0; i < recv_table->m_props; ++i ) {
		auto prop = &recv_table->m_props_pointer[i];

		if ( !prop || isdigit( prop->m_var_name[0] ) )
			continue;
		if ( strcmp( _( "baseclass" ), prop->m_var_name ) == 0 )
			continue;

		if ( prop->m_recv_type == dpt_datatable && prop->m_data_table ) {
			table.m_child_tables.emplace_back( load_table( prop->m_data_table ) );
			table.m_child_tables.back( ).m_offset = prop->m_offset;
			table.m_child_tables.back( ).m_prop = prop;
		}
		else {
			table.m_child_props.emplace_back( prop );
		}
	}
	return table;
}

uint32_t net_vars::get_offset( const std::string& table_name, const std::string& prop_name ) {
	auto result = 0u;
	for ( const auto& table : m_database ) {
		if ( table.m_name == table_name ) {
			result = get_offset( table, prop_name );
			if ( result != 0 )
				return result;
		}
	}
	return 0;
}

uint32_t net_vars::get_offset( const net_var_table_t& table, const std::string& prop_name ) {
	for ( const auto& prop : table.m_child_props ) {
		if ( strncmp( prop->m_var_name, prop_name.data( ), prop_name.size( ) ) == 0 ) {
			return table.m_offset + prop->m_offset;
		}
	}
	for ( const auto& child : table.m_child_tables ) {
		const auto prop_offset = get_offset( child, prop_name );
		if ( prop_offset != 0 )
			return table.m_offset + prop_offset;
	}
	for ( const auto& child : table.m_child_tables ) {
		if ( strncmp( child.m_prop->m_var_name, prop_name.data( ), prop_name.size( ) ) == 0 ) {
			return table.m_offset + child.m_offset;
		}
	}
	return 0;
}

recv_prop_t* net_vars::get_net_var_prop( const std::string& table_name, const std::string& prop_name ) {
	recv_prop_t* result = nullptr;
	for ( const auto& table : m_database ) {
		if ( table.m_name == table_name ) {
			result = get_net_var_prop( table, prop_name );
		}
	}
	return result;
}

recv_prop_t* net_vars::get_net_var_prop( const net_var_table_t& table, const std::string& prop_name ) {
	for ( const auto& prop : table.m_child_props ) {
		if ( strncmp( prop->m_var_name, prop_name.data( ), prop_name.size( ) ) == 0 ) {
			return prop;
		}
	}
	for ( const auto& child : table.m_child_tables ) {
		const auto prop = get_net_var_prop( child, prop_name );
		if ( prop != nullptr )
			return prop;
	}
	for ( const auto& child : table.m_child_tables ) {
		if ( strncmp( child.m_prop->m_var_name, prop_name.data( ), prop_name.size( ) ) == 0 ) {
			return child.m_prop;
		}
	}
	return nullptr;
}