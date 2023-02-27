#ifndef VALVE_NETVARS
#define VALVE_NETVARS

#include "../../../deps/fnv1a/fnv1a.hh"
#include "../misc/data_map.hh"
#include "../misc/dt_recv.hh"
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class c_net_vars {
    std::unordered_map< std::size_t, std::size_t > m_offsets{ };
    recv_table_t* m_recv_tables[ 1024 ]{ };
    int m_recv_tables_amount{ };
public:
    void init( );
    void add_table( std::string_view name, const recv_table_t* recv_table, std::size_t offset = 0 );

    [[nodiscard]] auto get( const std::string& name, const int offset = 0 ) {
        return m_offsets[ fnv1a::hash( name ) ] + offset;
    }
};

inline const std::unique_ptr< c_net_vars > g_net_vars{ new c_net_vars( ) };

#define NETVAR( type, name, table )                                                               \
    [[nodiscard]] __forceinline std::add_lvalue_reference_t< type > name( ) const {                                             \
        static const auto offset = g_net_vars->get( table );                                      \
        return *reinterpret_cast< type* >( reinterpret_cast< std::uintptr_t >( this ) + offset ); \
    }

#define NETVAR_OFFSET( type, name, table, add_offset )                                            \
    [[nodiscard]] __forceinline std::add_lvalue_reference_t< type > name( ) const {                                             \
        static const auto offset = g_net_vars->get( table, add_offset );                          \
        return *reinterpret_cast< type* >( reinterpret_cast< std::uintptr_t >( this ) + offset ); \
    }

#endif