#ifndef MEMORY_H
#define MEMORY_H

#include "address.hh"
#include "pattern.hh"
#include <memory>

class c_mem final {
public:
    template < typename T = c_address > [[nodiscard]] T get_vfunc( const c_address pointer, std::size_t index ) {
        return ( T )( pointer.to< T* >( )[ index ] );
    }

    [[nodiscard]] static auto get_virtual( void* instance, const std::size_t index ) {
        return reinterpret_cast< void* >( ( *static_cast< std::uintptr_t** >( instance ) )[ index ] );
    }
};

inline const std::unique_ptr< c_mem > g_mem{ new c_mem( ) };

#define OFFSET( type, name, offset )                                                              \
    [[nodiscard]] __forceinline std::add_lvalue_reference_t< type > name( ) const {                                             \
        return *reinterpret_cast< type* >( reinterpret_cast< std::uintptr_t >( this ) + offset ); \
    }

#define VFUNC( type, function_name, index, ... )                             \
    [[nodiscard]] __forceinline auto function_name {                         \
        return g_mem->get_vfunc< type >( this, index )( this, __VA_ARGS__ ); \
    }

#endif