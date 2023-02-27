#ifndef MEMORY_ADR_H
#define MEMORY_ADR_H

#include <cstddef>
#include <cstdint>

class c_address {
public:
    c_address( ) = default;
    c_address( const std::uintptr_t address ) : m_address{ address } {}
    c_address( void* address ) : m_address{ reinterpret_cast< std::uintptr_t >( address ) } {}

    operator std::uintptr_t( ) const { return m_address; }
    operator void*( ) const { return reinterpret_cast< void* >( m_address ); }

    bool operator==( const c_address address ) const { return as< std::uintptr_t >( ) == address.as< std::uintptr_t >( ); }
    bool operator!=( const c_address address ) const { return as< std::uintptr_t >( ) != address.as< std::uintptr_t >( ); }

    [[nodiscard]] bool is_valid( ) const { return m_address && this; }

    template < typename T = c_address >
    [[nodiscard]] T to( ) const { return *( T* )m_address; }

    template < typename T = c_address >
    [[nodiscard]] T as( ) const { return m_address ? T( m_address ) : T( ); }

    template < typename T = c_address >
    [[nodiscard]] T at( const std::ptrdiff_t offset ) const { return m_address ? *( T* )( m_address + offset ) : T( ); }

    template < typename T = c_address >
	[[nodiscard]] T offset( const std::ptrdiff_t offset ) const { return m_address ? T( m_address + offset ) : T( ); }

    template < typename T = c_address >
	[[nodiscard]] T get( std::ptrdiff_t dereferences = 1 ) const {
        std::ptrdiff_t object = m_address;

        while ( dereferences-- ) {
            if ( !object )
                break;

            object = *reinterpret_cast< std::ptrdiff_t* >( object );
        }

        return ( T )object;
    }

    template < typename T = c_address >
	[[nodiscard]] T relative( const std::ptrdiff_t offset = 0x1 ) const {
        std::ptrdiff_t new_address = m_address + offset;
        std::int32_t relative_offset = *reinterpret_cast< std::int32_t* >( new_address );

        if ( !relative_offset )
            return T( );

        return ( T )( new_address + 4 + relative_offset );
    }

    template < typename T = std::ptrdiff_t >
    void set( const T& value ) const {
        *( T* )m_address = value;
    }
private:
    std::uintptr_t m_address{ };
};

#endif