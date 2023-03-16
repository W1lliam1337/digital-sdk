#ifndef VALVE_MODULES
#define VALVE_MODULES

#include "../../../deps/deps.hh"
#include "../../utils/utils.hh"
#include <ranges>

class c_loaded_module : public c_pattern {
public:
    c_loaded_module( ) = default;
    c_loaded_module( const std::size_t loaded_module ) : c_pattern( loaded_module ) {}

    bool add_address( std::string_view name, std::string_view pattern, bool relative = false );
    bool hook_function( std::string_view name, LPVOID custom_function, LPVOID target = nullptr );
    bool valid_addresses( );
    void unload_functions( );

    c_address get_address( const std::string_view name ) { return m_addresses[ fnv1a::hash( name ) ]; }

    template < typename T > T get( const std::string_view name ) { return ( T ) m_originals[ fnv1a::hash( name ) ]; }
private:
    std::unordered_map< std::size_t, void* > m_originals{ };
    std::unordered_map< std::size_t, c_address > m_addresses{ };
};

class c_modules final {
    bool get_addresses( );
public:
    bool init( );
    bool get( );

	// TODO: create a struct for this or something like that - g_modules->client( )
    c_loaded_module m_client_dll{ }, m_gameoverlayrenderer_dll{ }, m_engine_dll{ }, m_steam_api_dll{ }, m_vstd_dll{ },
        m_vphysics_dll{ }, m_data_cahe_dll{ }, m_vguimatsurface_dll{ }, m_materialsystem_dll{ }, m_shaderapidx9_dll{ },
        m_serverbrowser_dll{ };
};

inline const std::unique_ptr< c_modules > g_modules{ new c_modules( ) };

#endif