#ifndef FNV1A_H
#define FNV1A_H

#include <cstdint>
#include <cstring>
#include <string_view>

namespace fnv1a {
    inline constexpr auto m_seed = 0x811C9DC5;
    inline constexpr auto m_prime = 0x1000193;

    inline std::uint32_t hash( const char* txt ) noexcept {
        auto hash = m_seed;

        for ( auto i = 0u; i < strlen( txt ); i++ ) {
            hash ^= txt[ i ];
            hash *= m_prime;
        }

        return hash;
    }

    inline std::uint32_t hash( const std::string_view& str ) noexcept {
        return hash( str.data( ) );
    }
}

#endif