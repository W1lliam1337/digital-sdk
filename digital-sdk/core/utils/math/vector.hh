#ifndef MATH_VECTOR
#define MATH_VECTOR

#include <cmath>
#include <numbers>

struct vec2_t {
    float x{ }, y{ };

    vec2_t( ) { x = y = 0.0f; }

    vec2_t( const float fx, const float fy ) {
        x = fx;
        y = fy;
    }

    vec2_t( float* v ) {
        x = v[ 0 ];
        y = v[ 1 ];
    }

    vec2_t( const float* v ) {
        x = v[ 0 ];
        y = v[ 1 ];
    }

    vec2_t( const vec2_t& v ) {
        x = v.x;
        y = v.y;
    }

    vec2_t& operator=( const vec2_t& v ) = default;
    float& operator[]( const int i ) { return reinterpret_cast< float* >( this )[ i ]; }
    float operator[]( const int i ) const { return ( ( float* ) this )[ i ]; }

    vec2_t& operator+=( const vec2_t& v ) {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2_t& operator-=( const vec2_t& v ) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2_t& operator*=( const vec2_t& v ) {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    vec2_t& operator/=( const vec2_t& v ) {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    vec2_t& operator+=( const float v ) {
        x += v;
        y += v;
        return *this;
    }

    vec2_t& operator-=( const float v ) {
        x -= v;
        y -= v;
        return *this;
    }

    vec2_t& operator*=( const float v ) {
        x *= v;
        y *= v;
        return *this;
    }

    vec2_t& operator/=( const float v ) {
        x /= v;
        y /= v;
        return *this;
    }

    vec2_t operator+( const vec2_t& v ) const { return { x + v.x, y + v.y }; }
    vec2_t operator-( const vec2_t& v ) const { return { x - v.x, y - v.y }; }
    vec2_t operator*( const vec2_t& v ) const { return { x * v.x, y * v.y }; }
    vec2_t operator/( const vec2_t& v ) const { return { x / v.x, y / v.y }; }
    vec2_t operator+( const float v ) const { return { x + v, y + v }; }
    vec2_t operator-( const float v ) const { return { x - v, y - v }; }
    vec2_t operator*( const float v ) const { return { x * v, y * v }; }
    vec2_t operator/( const float v ) const { return { x / v, y / v }; }
    operator bool( ) const noexcept { return !is_zero( ); }

    [[nodiscard]] float length( ) const { return std::sqrt( x * x + y * y ); }
    [[nodiscard]] float length_sqr( ) const { return ( x * x + y * y ); }
    [[nodiscard]] float dist_to( const vec2_t& v ) const { return ( *this - v ).length( ); }
    [[nodiscard]] float dist_to_sqr( const vec2_t& v ) const { return ( *this - v ).length_sqr( ); }
    [[nodiscard]] float dot( const vec2_t& v ) const { return x * v.x + y * v.y; }

    [[nodiscard]] bool is_zero( ) const {
        return x > -FLT_EPSILON && x < FLT_EPSILON && y > -FLT_EPSILON && y < FLT_EPSILON;
    }
};

struct vec3_t {
    vec3_t( const float* clr ) {
        x = clr[ 0 ];
        y = clr[ 1 ];
        z = clr[ 2 ];
    }

    vec3_t( ) {
        x = 0;
        y = 0;
        z = 0;
    }

    vec3_t( const float fx, const float fy, const float fz ) {
        x = fx;
        y = fy;
        z = fz;
    }

    void init( const float ix = 0.0f, const float iy = 0.0f, const float iz = 0.0f ) {
        x = ix;
        y = iy;
        z = iz;
    }

    vec3_t operator*( const float fl ) { return { x * fl, y * fl, z * fl }; }
    vec3_t operator*( const vec3_t& v ) { return { x * v.x, y * v.y, z * v.z }; }

    float& operator[]( const int i ) { return reinterpret_cast< float* >( this )[ i ]; }
    float operator[]( const int i ) const { return ( ( float* ) this )[ i ]; }

    bool operator==( const vec3_t& src ) const { return src.x == x && src.y == y && src.z == z; }
    bool operator!=( const vec3_t& src ) const { return src.x != x || src.y != y || src.z != z; }

    vec3_t& operator+=( const vec3_t& v ) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3_t& operator-=( const vec3_t& v ) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    vec3_t& operator*=( const float fl ) {
        x *= fl;
        y *= fl;
        z *= fl;
        return *this;
    }

    vec3_t& operator*=( const vec3_t& v ) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    vec3_t& operator/=( const vec3_t& v ) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    vec3_t& operator+=( const float fl ) {
        x += fl;
        y += fl;
        z += fl;
        return *this;
    }

    vec3_t& operator/=( const float fl ) {
        x /= fl;
        y /= fl;
        z /= fl;
        return *this;
    }

    vec3_t& operator-=( const float fl ) {
        x -= fl;
        y -= fl;
        z -= fl;
        return *this;
    }

    vec3_t& operator=( const vec3_t& other ) = default;
    vec3_t operator-( ) const { return { -x, -y, -z }; }
    vec3_t operator+( const vec3_t& v ) const { return { x + v.x, y + v.y, z + v.z }; }
    vec3_t operator+( const float fl ) const { return { x + fl, y + fl, z + fl }; }
    vec3_t operator-( const vec3_t& v ) const { return { x - v.x, y - v.y, z - v.z }; }
    vec3_t operator-( const float fl ) const { return { x - fl, y - fl, z - fl }; }
    vec3_t operator*( const float fl ) const { return { x * fl, y * fl, z * fl }; }
    vec3_t operator*( const vec3_t& v ) const { return { x * v.x, y * v.y, z * v.z }; }
    vec3_t operator/( const float fl ) const { return { x / fl, y / fl, z / fl }; }
    vec3_t operator/( const vec3_t& v ) const { return { x / v.x, y / v.y, z / v.z }; }

    [[nodiscard]] bool is_zero( ) const {
        return x > -FLT_EPSILON && x < FLT_EPSILON && y > -FLT_EPSILON && y < FLT_EPSILON;
    }

    float normalize_in_place( ) {
        const float length = this->length( );
        const float radius = 1.0f / ( length + std::numeric_limits< float >::epsilon( ) );

        this->x *= radius;
        this->y *= radius;
        this->z *= radius;

        return length;
    }

    [[nodiscard]] vec3_t normalized( ) const {
        auto ret = *this;
        ret.normalize( );
        return ret;
    }

    [[nodiscard]] float normalize( ) {
        const auto len = length( );
        if ( len ) {
            x /= len;
            y /= len;
            z /= len;
        }
        return len;
    }

    [[nodiscard]] float dist_to( const vec3_t& v_other ) const {
        vec3_t delta;

        delta.x = x - v_other.x;
        delta.y = y - v_other.y;
        delta.z = z - v_other.z;

        return delta.length( );
    }

    [[nodiscard]] float dist_to_sqr( const vec3_t& v_other ) const {
        vec3_t delta;

        delta.x = x - v_other.x;
        delta.y = y - v_other.y;
        delta.z = z - v_other.z;

        return delta.length_sqr( );
    }

    [[nodiscard]] float dot( const vec3_t& other ) const { return x * other.x + y * other.y + z * other.z; }

    static void vector_cross_product( const vec3_t& a, const vec3_t& b, vec3_t& result ) {
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
    }

    [[nodiscard]] vec3_t cross( const vec3_t& v_other ) const {
        vec3_t res;
        vector_cross_product( *this, v_other, res );
        return res;
    }

    [[nodiscard]] vec3_t to_angle( ) const {
        return { std::atan2( -z, std::hypot( x, y ) ) * ( 180.0f / std::numbers::pi_v< float > ),
                 std::atan2( y, x ) * ( 180.0f / std::numbers::pi_v< float > ), 0.0f };
    }

    [[nodiscard]] float length( ) const { return std::sqrt( x * x + y * y + z * z ); }
    [[nodiscard]] float length_sqr( ) const { return x * x + y * y + z * z; }
    [[nodiscard]] float length_2d_sqr( ) const { return x * x + y * y; }
    [[nodiscard]] float length_2d( ) const { return std::sqrt( x * x + y * y ); }

    float x{ }, y{ }, z{ };
};

struct __declspec( align( 16 ) ) vector_aligned_t : vec3_t {
    vector_aligned_t( ) = default;
    vector_aligned_t( const float x, const float y, const float z ) { init( x, y, z ); }
    explicit vector_aligned_t( const vec3_t& other ) { init( other.x, other.y, other.z ); }

    vector_aligned_t& operator=( const vec3_t& other ) {
        init( other.x, other.y, other.z );
        return *this;
    }

    vector_aligned_t& operator=( const vector_aligned_t& other ) {
        init( other.x, other.y, other.z );
        return *this;
    }

    float w{ };
};

#endif