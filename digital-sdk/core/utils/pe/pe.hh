#ifndef PE_H
#define PE_H

#include <memory>
#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <winternl.h>
#include <TlHelp32.h>
#include "../../../deps/fnv1a/fnv1a.hh"

class c_pe {
    std::unordered_map< std::size_t, std::size_t > m_loaded_modules{ };
public:
    bool init( );
    [[nodiscard]] std::size_t export_fn( std::size_t base, std::size_t fn_hash, bool in_memory = false ) const;

    std::size_t get( const std::string_view name ) { return m_loaded_modules[ fnv1a::hash( name ) ]; }
};

inline const std::unique_ptr< c_pe > g_pe{ new c_pe( ) };

#endif