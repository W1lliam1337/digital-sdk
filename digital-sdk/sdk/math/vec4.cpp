#include "vec4.h"

#include <cmath>
#include <limits>

void vector_copy( const c_vec4& src, c_vec4& dst ) {
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	dst.w = src.w;
}

void vector_lerp( const c_vec4& src1, const c_vec4& src2, const vec_t t, c_vec4& dest ) {
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
	dest.w = src1.w + (src2.w - src1.w) * t;
}

float vector_length( const c_vec4& v ) {
	return sqrt( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w );
}

vec_t normalize_vector( c_vec4& v ) {
	const vec_t l = v.length( );
	if ( l != 0.0f ) {
		v /= l;
	}
	else {
		v.x = v.y = v.z = v.w = 0.0f;
	}
	return l;
}

c_vec4::c_vec4( void ) {
	invalidate( );
}

c_vec4::c_vec4( const vec_t X, const vec_t Y, const vec_t Z, const vec_t W ) {
	x = X;
	y = Y;
	z = Z;
	w = W;
}

c_vec4::c_vec4( vec_t* clr ) {
	x = clr[0];
	y = clr[1];
	z = clr[2];
	w = clr[3];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void c_vec4::init( const vec_t ix, const vec_t iy, const vec_t iz, const vec_t iw ) {
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}

void c_vec4::random( const vec_t min_val, const vec_t max_val ) {
	x = min_val + static_cast<float>(rand( )) / RAND_MAX * (max_val - min_val);
	y = min_val + static_cast<float>(rand( )) / RAND_MAX * (max_val - min_val);
	z = min_val + static_cast<float>(rand( )) / RAND_MAX * (max_val - min_val);
	w = min_val + static_cast<float>(rand( )) / RAND_MAX * (max_val - min_val);
}

// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
void c_vec4::zero( ) {
	x = y = z = w = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

c_vec4& c_vec4::operator=( const c_vec4& other ) {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
vec_t& c_vec4::operator[]( const int i ) {
	return reinterpret_cast<vec_t*>(this)[i];
}

vec_t c_vec4::operator[]( const int i ) const {
	return ((vec_t*) this)[i];
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
vec_t* c_vec4::base( ) {
	return reinterpret_cast<vec_t*>(this);
}

const vec_t* c_vec4::base( ) const {
	return reinterpret_cast<const vec_t*>(this);
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool c_vec4::is_valid( ) const {
	return !isinf( x ) && !isinf( y ) && !isinf( z ) && !isinf( w );
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

void c_vec4::invalidate( ) {
	//#ifdef _DEBUG
	//#ifdef VECTOR_PARANOIA
	x = y = z = w = std::numeric_limits<float>::infinity( );
	//#endif
	//#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool c_vec4::operator==( const c_vec4& src ) const {
	return src.x == x && src.y == y && src.z == z && src.w == w;
}

bool c_vec4::operator!=( const c_vec4& src ) const {
	return src.x != x || src.y != y || src.z != z || src.w != w;
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
void c_vec4::copy_to_array( float* rgfl ) const {
	rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
	rgfl[3] = w;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------
// #pragma message("TODO: these should be SSE")

void c_vec4::negate( ) {
	x = -x;
	y = -y;
	z = -z;
	w = -w;
}

// Get the component of this vector parallel to some other given vector
c_vec4 c_vec4::project_onto( const c_vec4& onto ) const {
	return onto * (this->dot( onto ) / onto.length_sqr( ));
}

// FIXME: Remove
// For backwards compatability
void c_vec4::mul_add( const c_vec4& a, const c_vec4& b, const float scalar ) {
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
	w = a.w + b.w * scalar;
}

c_vec4 vector_lerp( const c_vec4& src1, const c_vec4& src2, vec_t t ) {
	c_vec4 result;
	vector_lerp( src1, src2, t, result );
	return result;
}

vec_t c_vec4::dot( const c_vec4& b ) const {
	return x * b.x + y * b.y + z * b.z + w * b.w;
}

void vector_clear( c_vec4& a ) {
	a.x = a.y = a.z = a.w = 0.0f;
}

vec_t c_vec4::length( void ) const {
	return sqrt( x * x + y * y + z * z + w * w );
}

// check a point against a box
bool c_vec4::within_aa_box( const c_vec4& boxmin, const c_vec4& boxmax ) const {
	return x >= boxmin.x && x <= boxmax.x &&
		y >= boxmin.y && y <= boxmax.y &&
		z >= boxmin.z && z <= boxmax.z &&
		w >= boxmin.w && w <= boxmax.w;
}

//-----------------------------------------------------------------------------
// Get the distance from this vector to the other one 
//-----------------------------------------------------------------------------
vec_t c_vec4::dist_to( const c_vec4& other ) const {
	const c_vec4 delta = *this - other;
	return delta.length( );
}

//-----------------------------------------------------------------------------
// Returns a vector with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------
c_vec4 c_vec4::Min( const c_vec4& other ) const {
	return {
		x < other.x ? x : other.x,
		y < other.y ? y : other.y,
		z < other.z ? z : other.z,
		w < other.w ? w : other.w
	};
}

c_vec4 c_vec4::Max( const c_vec4& other ) const {
	return {
		x > other.x ? x : other.x,
		y > other.y ? y : other.y,
		z > other.z ? z : other.z,
		w > other.w ? w : other.w
	};
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

c_vec4 c_vec4::operator-( void ) const {
	return { -x, -y, -z, -w };
}

c_vec4 c_vec4::operator+( const c_vec4& v ) const {
	return { x + v.x, y + v.y, z + v.z, w + v.w };
}

c_vec4 c_vec4::operator-( const c_vec4& v ) const {
	return { x - v.x, y - v.y, z - v.z, w - v.w };
}

c_vec4 c_vec4::operator*( const float fl ) const {
	return { x * fl, y * fl, z * fl, w * fl };
}

c_vec4 c_vec4::operator*( const c_vec4& v ) const {
	return { x * v.x, y * v.y, z * v.z, w * v.w };
}

c_vec4 c_vec4::operator/( const float fl ) const {
	return { x / fl, y / fl, z / fl, w / fl };
}

c_vec4 c_vec4::operator/( const c_vec4& v ) const {
	return { x / v.x, y / v.y, z / v.z, w / v.w };
}

c_vec4 operator*( const float fl, const c_vec4& v ) {
	return v * fl;
}
