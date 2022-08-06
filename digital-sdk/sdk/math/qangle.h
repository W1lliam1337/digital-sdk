#pragma once
#include <cmath>

struct qangle_t {
	qangle_t( void ) {
		init( );
	}

	qangle_t( const float x, const float y, const float z ) {
		init( x, y, z );
	}

	qangle_t( const float* clr ) {
		init( clr[0], clr[1], clr[2] );
	}

	void init( const float ix = 0.0f, const float iy = 0.0f, const float iz = 0.0f ) {
		x = ix;
		y = iy;
		z = iz;
	}

	float operator[]( const int i ) const {
		return ((float*) this)[i];
	}

	float& operator[]( const int i ) {
		return reinterpret_cast<float*>(this)[i];
	}

	qangle_t& operator+=( const qangle_t& v ) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	qangle_t& operator-=( const qangle_t& v ) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	qangle_t& operator*=( const float fl ) {
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	qangle_t& operator*=( const qangle_t& v ) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	qangle_t& operator/=( const qangle_t& v ) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	qangle_t& operator+=( const float fl ) {
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	qangle_t& operator/=( const float fl ) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	qangle_t& operator-=( const float fl ) {
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	qangle_t& operator=( const qangle_t& other ) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	qangle_t operator-( void ) const {
		return { -x, -y, -z };
	}

	qangle_t operator+( const qangle_t& v ) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	qangle_t operator-( const qangle_t& v ) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	qangle_t operator*( const float fl ) const {
		return { x * fl, y * fl, z * fl };
	}

	qangle_t operator*( const qangle_t& v ) const {
		return { x * v.x, y * v.y, z * v.z };
	}

	qangle_t operator/( const float fl ) const {
		return { x / fl, y / fl, z / fl };
	}

	qangle_t operator/( const qangle_t& v ) const {
		return { x / v.x, y / v.y, z / v.z };
	}

	[[nodiscard]] float length( ) const {
		return sqrt( x * x + y * y + z * z );
	}

	[[nodiscard]] float length_sqr( void ) const {
		return (x * x + y * y + z * z);
	}

	[[nodiscard]] bool is_zero( const float tolerance = 0.01f ) const {
		return (x > -tolerance && x < tolerance&&
				 y > -tolerance && y < tolerance&&
				 z > -tolerance && z < tolerance);
	}

	[[nodiscard]] __inline bool is_valid( ) const {
		return std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z );
	}

	auto clamp( ) {
		while ( this->x < -89.0f ) {
			this->x += 89.0f;
		}

		if ( this->x > 89.0f ) {
			this->x = 89.0f;
		}

		while ( this->y < -180.0f ) {
			this->y += 360.0f;
		}

		while ( this->y > 180.0f ) {
			this->y -= 360.0f;
		}

		this->z = 0.0f;
		return *this;
	}

	[[nodiscard]] float normalize( ) const {
		qangle_t res = *this;
		const float l = res.length( );
		if ( l != 0.0f ) {
			res /= l;
		}
		else {
			res[0] = res[1] = res[2] = 0.0f;
		}
		return l;
	}

	float x{}, y{}, z{};
};

inline qangle_t operator*( const float lhs, const qangle_t& rhs ) {
	return rhs * lhs;
}

inline qangle_t operator/( const float lhs, const qangle_t& rhs ) {
	return rhs / lhs;
}
