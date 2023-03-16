#ifndef MATH_MATRIX
#define MATH_MATRIX

#include "vector.hh"
#include <span>

template < typename type_t >
concept arithmetic = std::is_arithmetic_v< type_t >;

template < arithmetic T, size_t R, size_t C > struct matrix_t {
    T m_value[ R ][ C ]{ };
    matrix_t( ) = default;

    matrix_t( std::initializer_list< T > values ) {
        auto it = values.begin( );
        for ( auto& row : m_value ) {
            for ( auto& element : row ) {
                element = *it;
                ++it;
            }
        }
    }

    matrix_t( const std::array< std::array< T, C >, R >& values ) : m_value( values ) {}

    void init( const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& origin ) {
        m_value = { { { x_axis.x, y_axis.x, z_axis.x, origin.x },
                      { x_axis.y, y_axis.y, z_axis.y, origin.y },
                      { x_axis.z, y_axis.z, z_axis.z, origin.z } } };
    }

    matrix_t( const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& origin ) {
        init( x_axis, y_axis, z_axis, origin );
    }

    auto operator[]( const std::size_t i ) { return m_value[ i ]; }
    const float* operator[]( const int i ) const { return m_value[ i ]; }

    void set_forward( const vec3_t& forward ) {
        m_value[ 0 ][ 0 ] = forward.x;
        m_value[ 1 ][ 0 ] = forward.y;
        m_value[ 2 ][ 0 ] = forward.z;
    }

    void set_left( const vec3_t& left ) {
        m_value[ 0 ][ 1 ] = left.x;
        m_value[ 1 ][ 1 ] = left.y;
        m_value[ 2 ][ 1 ] = left.z;
    }

    void set_up( const vec3_t& up ) {
        m_value[ 0 ][ 2 ] = up.x;
        m_value[ 1 ][ 2 ] = up.y;
        m_value[ 2 ][ 2 ] = up.z;
    }

    void set_origin( const vec3_t& origin ) {
        m_value[ 0 ][ 3 ] = origin.x;
        m_value[ 1 ][ 3 ] = origin.y;
        m_value[ 2 ][ 3 ] = origin.z;
    }

    float* base( ) { return &m_value[ 0 ][ 0 ]; }
    [[nodiscard]] const float* base( ) const { return &m_value[ 0 ][ 0 ]; }
};

using matrix3x4_t = matrix_t< float, 3, 4 >;
using matrix4x4_t = matrix_t< float, 4, 4 >;

#endif