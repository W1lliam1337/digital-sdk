#pragma once
#include <string>
#include <vector>
#include "../../includes.h"

struct recv_prop_t;
struct recv_table_t;

struct net_var_table_t {
	std::string m_name{};
	recv_prop_t* m_prop{};
	uint32_t m_offset{};
	std::vector<recv_prop_t*> m_child_props{};
	std::vector<net_var_table_t> m_child_tables{};
};

namespace net_vars {
	void init( );

	uint32_t      get_offset( const std::string& table_name, const std::string& prop_name );
	recv_prop_t* get_net_var_prop( const std::string& table_name, const std::string& prop_name );

	net_var_table_t  load_table( const recv_table_t* recv_table );

	uint32_t      get_offset( const net_var_table_t& table, const std::string& prop_name );
	recv_prop_t* get_net_var_prop( const net_var_table_t& table, const std::string& prop_name );
}

#define GET_NETVAR_OFFSET( type, name, table, netvar, offset ) \
    [[nodiscard]] __forceinline type& name( ) const { \
        static const int _##name = net_vars::get_offset( table, netvar ) + offset; \
        return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>( this ) + _##name); \
    }

#define GET_NETVAR( type, name, table, netvar ) \
    [[nodiscard]] __forceinline type& name( ) const { \
        static const int _##name = net_vars::get_offset( table, netvar ); \
        return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>( this ) + _##name); \
    }

#define GET_OFFSET( type, name, offset ) \
	[[nodiscard]] __forceinline type& name( ) const { \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>( this ) + offset); \
	}