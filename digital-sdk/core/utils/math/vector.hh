#ifndef MATH_VECTOR
#define MATH_VECTOR

#include <cmath>
#include <numbers>

class c_vec2 {
public:
    float x{ }, y{ };

    constexpr c_vec2( ) { x = y = 0.0f; }

    c_vec2( const float X, const float Y ) {
        x = X;
        y = Y;
    }

    c_vec2( float* v ) {
        x = v[ 0 ];
        y = v[ 1 ];
    }

    c_vec2( const float* v ) {
        x = v[ 0 ];
        y = v[ 1 ];
    }

    c_vec2( const c_vec2& v ) {
        x = v.x;
        y = v.y;
    }

    c_vec2& operator=( const c_vec2& v ) = default;
    float& operator[]( const int i ) { return reinterpret_cast< float* >( this )[ i ]; }
    float operator[]( const int i ) const { return ( ( float* ) this )[ i ]; }

    c_vec2& operator+=( const c_vec2& v ) {
        x += v.x;
        y += v.y;
        return *this;
    }

    c_vec2& operator-=( const c_vec2& v ) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    c_vec2& operator*=( const c_vec2& v ) {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    c_vec2& operator/=( const c_vec2& v ) {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    c_vec2& operator+=( const float v ) {
        x += v;
        y += v;
        return *this;
    }

    c_vec2& operator-=( const float v ) {
        x -= v;
        y -= v;
        return *this;
    }

    c_vec2& operator*=( const float v ) {
        x *= v;
        y *= v;
        return *this;
    }

    c_vec2& operator/=( const float v ) {
        x /= v;
        y /= v;
        return *this;
    }

    c_vec2 operator+( const c_vec2& v ) const { return { x + v.x, y + v.y }; }
    c_vec2 operator-( const c_vec2& v ) const { return { x - v.x, y - v.y }; }
    c_vec2 operator*( const c_vec2& v ) const { return { x * v.x, y * v.y }; }
    c_vec2 operator/( const c_vec2& v ) const { return { x / v.x, y / v.y }; }
    c_vec2 operator+( const float v ) const { return { x + v, y + v }; }
    c_vec2 operator-( const float v ) const { return { x - v, y - v }; }
    c_vec2 operator*( const float v ) const { return { x * v, y * v }; }
    c_vec2 operator/( const float v ) const { return { x / v, y / v }; }
    operator bool( ) const noexcept { return !is_zero( ); }

    void set( const float X = 0.0f, const float Y = 0.0f ) {
        x = X;
        y = Y;
    }

    [[nodiscard]] float length( ) const { return std::sqrt( x * x + y * y ); }

    [[nodiscard]] float length_sqr( ) const { return ( x * x + y * y ); }

    [[nodiscard]] float dist_to( const c_vec2& v ) const { return ( *this - v ).length( ); }

    [[nodiscard]] float dist_to_sqr( const c_vec2& v ) const { return ( *this - v ).length_sqr( ); }

    [[nodiscard]] float dot( const c_vec2& v ) const { return x * v.x + y * v.y; }

    [[nodiscard]] bool is_zero( void ) const {
        return ( x > -FLT_EPSILON && x < FLT_EPSILON && y > -FLT_EPSILON && y < FLT_EPSILON );
    }
};

class c_vec3 {
public:
    c_vec3( const float* clr ) {
        x = clr[ 0 ];
        y = clr[ 1 ];
        z = clr[ 2 ];
    }

    c_vec3( ) {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    c_vec3( const float fx, const float fy, const float fz ) {
        this->x = fx;
        this->y = fy;
        this->z = fz;
    }

    void init( const float ix = 0.0f, const float iy = 0.0f, const float iz = 0.0f ) {
        x = ix;
        y = iy;
        z = iz;
    }

    c_vec3 operator*( const float fl ) { return { x * fl, y * fl, z * fl }; }
    c_vec3 operator*( const c_vec3& v ) { return { x * v.x, y * v.y, z * v.z }; }

    float& operator[]( const int i ) { return reinterpret_cast< float* >( this )[ i ]; }
    float operator[]( const int i ) const { return ( ( float* ) this )[ i ]; }

    bool operator==( const c_vec3& src ) const { return src.x == x && src.y == y && src.z == z; }
    bool operator!=( const c_vec3& src ) const { return src.x != x || src.y != y || src.z != z; }

    c_vec3& operator+=( const c_vec3& v ) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    c_vec3& operator-=( const c_vec3& v ) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    c_vec3& operator*=( const float fl ) {
        x *= fl;
        y *= fl;
        z *= fl;
        return *this;
    }

    c_vec3& operator*=( const c_vec3& v ) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    c_vec3& operator/=( const c_vec3& v ) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    c_vec3& operator+=( const float fl ) {
        x += fl;
        y += fl;
        z += fl;
        return *this;
    }

    c_vec3& operator/=( const float fl ) {
        x /= fl;
        y /= fl;
        z /= fl;
        return *this;
    }

    c_vec3& operator-=( const float fl ) {
        x -= fl;
        y -= fl;
        z -= fl;
        return *this;
    }

    c_vec3& operator=( const c_vec3& other ) = default;
    c_vec3 operator-( ) const { return { -x, -y, -z }; }
    c_vec3 operator+( const c_vec3& v ) const { return { x + v.x, y + v.y, z + v.z }; }
    c_vec3 operator+( const float fl ) const { return { x + fl, y + fl, z + fl }; }
    c_vec3 operator-( const c_vec3& v ) const { return { x - v.x, y - v.y, z - v.z }; }
    c_vec3 operator-( const float fl ) const { return { x - fl, y - fl, z - fl }; }
    c_vec3 operator*( const float fl ) const { return { x * fl, y * fl, z * fl }; }
    c_vec3 operator*( const c_vec3& v ) const { return { x * v.x, y * v.y, z * v.z }; }
    c_vec3 operator/( const float fl ) const { return { x / fl, y / fl, z / fl }; }
    c_vec3 operator/( const c_vec3& v ) const { return { x / v.x, y / v.y, z / v.z }; }

    void mul( const float scalar ) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    [[nodiscard]] bool is_zero( ) const { return !x && !y && !z; }

    [[nodiscard]] bool is_valid( ) const {
        return isfinite( x ) && isfinite( y ) && isfinite( z ) && x < 9999999 && y < 9999999 && z < 9999999;
    }

    float normalize_in_place( ) {
        const float length = this->length( );
        const float radius = 1.0f / ( length + std::numeric_limits< float >::epsilon( ) );

        this->x *= radius;
        this->y *= radius;
        this->z *= radius;

        return length;
    }

    [[nodiscard]] c_vec3 normalized( ) const {
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

    [[nodiscard]] float dist_to( const c_vec3& v_other ) const {
        c_vec3 delta;

        delta.x = x - v_other.x;
        delta.y = y - v_other.y;
        delta.z = z - v_other.z;

        return delta.length( );
    }

    [[nodiscard]] float dist_to_sqr( const c_vec3& v_other ) const {
        c_vec3 delta;

        delta.x = x - v_other.x;
        delta.y = y - v_other.y;
        delta.z = z - v_other.z;

        return delta.length_sqr( );
    }

    [[nodiscard]] float dot( const c_vec3& other ) const { return x * other.x + y * other.y + z * other.z; }

    static void vector_cross_product( const c_vec3& a, const c_vec3& b, c_vec3& result ) {
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
    }

    [[nodiscard]] c_vec3 cross( const c_vec3& v_other ) const {
        c_vec3 res;
        vector_cross_product( *this, v_other, res );
        return res;
    }

    [[nodiscard]] c_vec3 to_angle( ) const {
        return { std::atan2( -z, std::hypot( x, y ) ) * ( 180.0f / std::numbers::pi_v< float > ),
                 std::atan2( y, x ) * ( 180.0f / std::numbers::pi_v< float > ), 0.0f };
    }

    void ma( const c_vec3& start, const float scale, const c_vec3& dir ) {
        x = start.x + dir.x * scale;
        y = start.y + dir.y * scale;
        z = start.z + dir.z * scale;
    }

    [[nodiscard]] float length( ) const { return sqrt( x * x + y * y + z * z ); }

    [[nodiscard]] float length_sqr( void ) const { return x * x + y * y + z * z; }

    [[nodiscard]] float length_2d_sqr( ) const { return x * x + y * y; }

    [[nodiscard]] float length_2d( ) const { return sqrt( x * x + y * y ); }

    float x{ }, y{ }, z{ };
};

class __declspec( align( 16 ) ) vector_aligned : public c_vec3 {
public:
    vector_aligned( void ) = default;

    vector_aligned( const float x, const float y, const float z ) { init( x, y, z ); }

    explicit vector_aligned( const c_vec3& other ) { init( other.x, other.y, other.z ); }

    vector_aligned& operator=( const c_vec3& other ) {
        init( other.x, other.y, other.z );
        return *this;
    }

    vector_aligned& operator=( const vector_aligned& other ) {
        init( other.x, other.y, other.z );
        return *this;
    }

    float w{ };
};

#endif