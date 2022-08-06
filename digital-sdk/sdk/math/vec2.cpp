#include "vec2.h"
#include <cmath>

c_vec2::c_vec2( void ) {
}

c_vec2::c_vec2( vec_t X, vec_t Y ) {
	x = X; y = Y;
}

c_vec2::c_vec2( vec_t* clr ) {
	x = clr[0]; y = clr[1];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void c_vec2::init( const vec_t ix, const vec_t iy ) {
	x = ix; y = iy;
}

void c_vec2::random( const float min_val, const float max_val ) {
	x = min_val + static_cast<float>(rand( )) / RAND_MAX * (max_val - min_val);
	y = min_val + static_cast<float>(rand( )) / RAND_MAX * (max_val - min_val);
}

void Vector2DClear( c_vec2& a ) {
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

c_vec2& c_vec2::operator=( const c_vec2& other ) {
	x = other.x; y = other.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

vec_t& c_vec2::operator[]( const int i ) {
	return reinterpret_cast<vec_t*>(this)[i];
}

vec_t c_vec2::operator[]( const int i ) const {
	return ((vec_t*) this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

vec_t* c_vec2::base( ) {
	return reinterpret_cast<vec_t*>(this);
}

vec_t const* c_vec2::base( ) const {
	return reinterpret_cast<vec_t const*>(this);
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool c_vec2::is_valid( ) const {
	return !isinf( x ) && !isinf( y );
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool c_vec2::operator==( const c_vec2& src ) const {
	return src.x == x && src.y == y;
}

bool c_vec2::operator!=( const c_vec2& src ) const {
	return src.x != x || src.y != y;
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

void vector_2d_copy( const c_vec2& src, c_vec2& dst ) {
	dst.x = src.x;
	dst.y = src.y;
}

void c_vec2::copy_to_array( float* rgfl ) const {
	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void c_vec2::negate( ) {
	x = -x; y = -y;
}

void vector_2d_add( const c_vec2& a, const c_vec2& b, c_vec2& c ) {
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

void vector_2d_subtract( const c_vec2& a, const c_vec2& b, c_vec2& c ) {
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

void vector_2d_multiply( const c_vec2& a, const vec_t b, c_vec2& c ) {
	c.x = a.x * b;
	c.y = a.y * b;
}

void vector_2d_multiply( const c_vec2& a, const c_vec2& b, c_vec2& c ) {
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}

void vector_2d_divide( const c_vec2& a, const vec_t b, c_vec2& c ) {
	const vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

void vector_2d_divide( const c_vec2& a, const c_vec2& b, c_vec2& c ) {
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

void vector_2dma( const c_vec2& start, const float s, const c_vec2& dir, c_vec2& result ) {
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// @fix-me: Remove
// For backwards compatability
void c_vec2::mul_add( const c_vec2& a, const c_vec2& b, const float scalar ) {
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

void vector_2d_lerp( const c_vec2& src1, const c_vec2& src2, const vec_t t, c_vec2& dest ) {
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
vec_t dot_product_2d( const c_vec2& a, const c_vec2& b ) {
	return a.x * b.x + a.y * b.y;
}

// for backwards compatability
vec_t c_vec2::dot( const c_vec2& other ) const {
	return dot_product_2d( *this, other );
}

vec_t vector_2d_normalize( c_vec2& v ) {
	const vec_t l = v.length( );
	if ( l != 0.0f ) {
		v /= l;
	}
	else {
		v.x = v.y = 0.0f;
	}
	return l;
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
vec_t vector_2d_length( const c_vec2& v ) {
	return sqrt( v.x * v.x + v.y * v.y );
}

vec_t c_vec2::normalize_in_place( ) {
	return vector_2d_normalize( *this );
}

bool c_vec2::is_length_greater_than( const float val ) const {
	return length_sqr( ) > val * val;
}

bool c_vec2::is_length_less_than( const float val ) const {
	return length_sqr( ) < val * val;
}

vec_t c_vec2::length( void ) const {
	return vector_2d_length( *this );
}

void vector_2d_min( const c_vec2& a, const c_vec2& b, c_vec2& result ) {
	result.x = a.x < b.x ? a.x : b.x;
	result.y = a.y < b.y ? a.y : b.y;
}

void vector_2d_max( const c_vec2& a, const c_vec2& b, c_vec2& result ) {
	result.x = a.x > b.x ? a.x : b.x;
	result.y = a.y > b.y ? a.y : b.y;
}

//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
void compute_closest_point_2d( const c_vec2& start, const float max_dist, const c_vec2& target, c_vec2* result ) {
	c_vec2 delta;
	vector_2d_subtract( target, start, delta );
	const float dist_sqr = delta.length_sqr( );
	if ( dist_sqr <= max_dist * max_dist ) {
		*result = target;
	}
	else {
		delta /= sqrt( dist_sqr );
		vector_2dma( start, max_dist, delta, *result );
	}
}

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

c_vec2 c_vec2::Min( const c_vec2& other ) const {
	return { x < other.x ? x : other.x, y < other.y ? y : other.y };
}

c_vec2 c_vec2::Max( const c_vec2& other ) const {
	return { x > other.x ? x : other.x, y > other.y ? y : other.y };
}

//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

c_vec2 c_vec2::operator-( void ) const {
	return { -x, -y };
}

c_vec2 c_vec2::operator+( const c_vec2& v ) const {
	c_vec2 res;
	vector_2d_add( *this, v, res );
	return res;
}

c_vec2 c_vec2::operator-( const c_vec2& v ) const {
	c_vec2 res;
	vector_2d_subtract( *this, v, res );
	return res;
}

c_vec2 c_vec2::operator*( const float fl ) const {
	c_vec2 res;
	vector_2d_multiply( *this, fl, res );
	return res;
}

c_vec2 c_vec2::operator*( const c_vec2& v ) const {
	c_vec2 res;
	vector_2d_multiply( *this, v, res );
	return res;
}

c_vec2 c_vec2::operator/( const float fl ) const {
	c_vec2 res;
	vector_2d_divide( *this, fl, res );
	return res;
}

c_vec2 c_vec2::operator/( const c_vec2& v ) const {
	c_vec2 res;
	vector_2d_divide( *this, v, res );
	return res;
}

c_vec2 operator*( const float fl, const c_vec2& v ) {
	return v * fl;
}