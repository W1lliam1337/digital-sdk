#pragma once
#include <cmath>
#include <numbers>

class c_vec3 {
public:
	c_vec3( const float* clr ) {
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	c_vec3( ) {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	};

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

	c_vec3 operator*( const float fl ) {
		return { x * fl, y * fl, z * fl };
	}

	c_vec3 operator*( const c_vec3& v ) {
		return { x * v.x, y * v.y, z * v.z };
	}

	__inline void mul( const float scalar ) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	float& operator[]( const int i ) {
		return reinterpret_cast<float*>(this)[i];
	}

	float operator[]( const int i ) const {
		return ((float*) this)[i];
	}

	[[nodiscard]] bool __inline is_zero( ) const {
		return !x && !y && !z;
	}

	[[nodiscard]] bool is_valid( ) const {
		return isfinite( x ) && isfinite( y ) && isfinite( z ) && x < 9999999 && y < 9999999 && z < 9999999;
	}

	c_vec3 clamp( ) {
		if ( x < -89.0f ) {
			x = -89.0f;
		}

		if ( x > 89.0f ) {
			x = 89.0f;
		}

		while ( y < -180.0f ) {
			y += 360.0f;
		}

		while ( y > 180.0f ) {
			y -= 360.0f;
		}

		z = 0.0f;
		return *this;
	}

	void __inline zero( ) {
		x = y = z = 0.0f;
	}

	bool operator==( const c_vec3& src ) const {
		return src.x == x && src.y == y && src.z == z;
	}

	bool operator!=( const c_vec3& src ) const {
		return src.x != x || src.y != y || src.z != z;
	}

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

	float normalize_in_place( ) {
		const float length = this->length( );
		const float radius = 1.0f / (length + std::numeric_limits<float>::epsilon( ));

		this->x *= radius;
		this->y *= radius;
		this->z *= radius;

		return length;
	}

	[[nodiscard]] c_vec3 normalized( ) const {
		c_vec3 res = *this;
		if ( const float l = res.length( ); l != 0.0f ) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	[[nodiscard]] float normalize( ) const {
		c_vec3 res = *this;
		const float l = res.length( );
		if ( l != 0.0f ) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return l;
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

	[[nodiscard]] float dot( const c_vec3& other ) const {
		return x * other.x + y * other.y + z * other.z;
	}

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
		return {
			std::atan2( -z, std::hypot( x, y ) ) * (180.0f / std::numbers::pi_v<float>),
			std::atan2( y, x ) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	[[nodiscard]] float length( ) const {
		return sqrt( x * x + y * y + z * z );
	}

	[[nodiscard]] float length_sqr( void ) const {
		return x * x + y * y + z * z;
	}

	[[nodiscard]] float length_2d_sqr( ) const {
		return x * x + y * y;
	}

	[[nodiscard]] float length_2d( ) const {
		return sqrt( x * x + y * y );
	}

	c_vec3& operator=( const c_vec3& other ) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	c_vec3 operator-( ) const {
		return { -x, -y, -z };
	}

	c_vec3 operator+( const c_vec3& v ) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	c_vec3 operator+( const float fl ) const {
		return { x + fl, y + fl, z + fl };
	}

	c_vec3 operator-( const c_vec3& v ) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	c_vec3 operator-( const float fl ) const {
		return { x - fl, y - fl, z - fl };
	}

	c_vec3 operator*( const float fl ) const {
		return { x * fl, y * fl, z * fl };
	}

	c_vec3 operator*( const c_vec3& v ) const {
		return { x * v.x, y * v.y, z * v.z };
	}

	c_vec3 operator/( const float fl ) const {
		return { x / fl, y / fl, z / fl };
	}

	c_vec3 operator/( const c_vec3& v ) const {
		return { x / v.x, y / v.y, z / v.z };
	}

	float x{}, y{}, z{};
};
