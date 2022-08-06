#pragma once

typedef float vec_t;
// 3D c_vec4
class c_vec4 {
public:
	// Members
	vec_t x{}, y{}, z{}, w{};

	// Construction/destruction:
	c_vec4( void );
	c_vec4( vec_t X, vec_t Y, vec_t Z, vec_t W );
	c_vec4( vec_t* clr );

	// Initialization
	void init( vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f );
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	[[nodiscard]] bool is_valid( ) const;
	void invalidate( );

	// array access...
	vec_t operator[]( int i ) const;
	vec_t& operator[]( int i );

	// Base address...
	vec_t* base( );
	[[nodiscard]] vec_t const* base( ) const;

	// Initialization methods
	void random( vec_t min_val, vec_t max_val );
	void zero( ); ///< zero out a vector

				 // equality
	bool operator==( const c_vec4& src ) const;
	bool operator!=( const c_vec4& src ) const;

	// arithmetic operations
	c_vec4& operator+=( const c_vec4& v ) {
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	c_vec4& operator-=( const c_vec4& v ) {
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	c_vec4& operator*=( const float fl ) {
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	c_vec4& operator*=( const c_vec4& v ) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	c_vec4& operator/=( const c_vec4& v ) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	// this ought to be an opcode.
	c_vec4& operator+=( float fl ) {
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	// this ought to be an opcode.
	c_vec4& operator/=( float fl ) {
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	c_vec4& operator-=( float fl ) {
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}

	// negate the vector components
	void negate( );

	// Get the vector's magnitude.
	[[nodiscard]] vec_t length( ) const;

	// Get the vector's magnitude squared.
	[[nodiscard]] vec_t length_sqr( void ) const {
		return x * x + y * y + z * z;
	}

	// return true if this vector is (0,0,0) within tolerance
	[[nodiscard]] bool is_zero( const float tolerance = 0.01f ) const {
		return x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance&&
			w > -tolerance && w < tolerance;
	}

	vec_t normalize_in_place( );
	[[nodiscard]] c_vec4 Normalized( ) const;
	[[nodiscard]] bool IsLengthGreaterThan( float val ) const;
	[[nodiscard]] bool IsLengthLessThan( float val ) const;

	// check if a vector is within the box defined by two other vectors
	bool within_aa_box( c_vec4 const& boxmin, c_vec4 const& boxmax ) const;

	// Get the distance from this vector to the other one.
	[[nodiscard]] vec_t dist_to( const c_vec4& other ) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	[[nodiscard]] vec_t dist_to_sqr( const c_vec4& other ) const {
		c_vec4 delta;

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;
		delta.w = w - other.w;

		return delta.length_sqr( );
	}

	// Copy
	void copy_to_array( float* rgfl ) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void mul_add( const c_vec4& a, const c_vec4& b, float scalar );

	// Dot product.
	[[nodiscard]] vec_t dot( const c_vec4& b ) const;

	// assignment
	c_vec4& operator=( const c_vec4& other );

	// 2d
	[[nodiscard]] vec_t length_2d( void ) const;
	[[nodiscard]] vec_t length_2d_sqr( void ) const;

	/// Get the component of this vector parallel to some other given vector
	c_vec4  project_onto( const c_vec4& onto ) const;

	// copy constructors
	// c_vec4(const c_vec4 &other);

	// arithmetic operations
	c_vec4 operator-( void ) const;

	c_vec4 operator+( const c_vec4& v ) const;
	c_vec4 operator-( const c_vec4& v ) const;
	c_vec4 operator*( const c_vec4& v ) const;
	c_vec4 operator/( const c_vec4& v ) const;
	c_vec4 operator*( float fl ) const;
	c_vec4 operator/( float fl ) const;

	// Returns a vector with the min or max in X, Y, and Z.
	[[nodiscard]] c_vec4 Min( const c_vec4& other ) const;
	[[nodiscard]] c_vec4 Max( const c_vec4& other ) const;
};