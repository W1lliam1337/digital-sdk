#include "pattern.hh"
#include "../console/console.hh"

c_address c_pattern::find( const std::string_view& pattern, const std::string_view& name ) const {
    const auto bytes = std::make_unique< int[] >( pattern.size( ) / 2 );
    const auto result = search_byte_array( bytes.get( ), build_byte_array( pattern.data( ), bytes.get( ) ) );

    if ( !result )
        log_print( "failed to find pattern: Func name: {} | {}", name.data( ), pattern.data( ) );

    return result;
}

std::size_t c_pattern::build_byte_array( const char* pattern, int* bytes ) {
    std::size_t count = 0;

    auto char_to_int = []( const char input ) {
        return std::isalnum( input ) ? std::stoi( std::string( 1, input ), nullptr, 16 ) : -1;
    };

    while ( pattern[ 0 ] && pattern[ 1 ] ) {
        if ( pattern[ 0 ] == '?' || pattern[ 1 ] == '?' ) {
            bytes[ count++ ] = -1;
            pattern += 1;
        } else if ( !std::isspace( pattern[ 0 ] ) && !std::isspace( pattern[ 1 ] ) ) {
            bytes[ count++ ] = char_to_int( pattern[ 0 ] ) * 16 + char_to_int( pattern[ 1 ] );
        }

        pattern += 1;
    }

    return count;
}

c_address c_pattern::search_byte_array( const int* bytes, const std::size_t size ) const {
    c_address result;
    const auto& bitmap = m_bitmap;
    const auto& end = m_size - size;

    for ( std::size_t i = 0; i <= end; ++i ) {
        bool found = true;
        for ( std::size_t j = 0; j < size; ++j ) {
            const bool byte_matches = bytes[ j ] == -1 || bitmap[ i + j ] == static_cast< std::uint8_t >( bytes[ j ] );
            if ( !byte_matches ) {
                found = false;
                break;
            }
        }
        if ( found ) {
            result = &bitmap[ i ];
        }
    }

    return result;
}