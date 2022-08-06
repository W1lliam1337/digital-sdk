#pragma once

typedef float vec_t;
// 2D Vector
class c_vec2 {
public:
	// Members
	vec_t x{}, y{};

	// Construction/destruction:
	c_vec2( void );
	c_vec2( vec_t X, vec_t Y );
	c_vec2( vec_t* clr );

	c_vec2( const c_vec2& vOther ) {
		x = vOther.x; y = vOther.y;
	}

	// Initialization
	void init( vec_t ix = 0.0f, vec_t iy = 0.0f );
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
	bool operator==( const c_vec2& src ) const;
	bool operator!=( const c_vec2& src ) const;

	// arithmetic operations
	c_vec2& operator+=( const c_vec2& v ) {
		x += v.x; y += v.y;
		return *this;
	}

	c_vec2& operator-=( const c_vec2& v ) {
		x -= v.x; y -= v.y;
		return *this;
	}

	c_vec2& operator*=( float fl ) {
		x *= fl;
		y *= fl;
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

	// this ought to be an opcode.
	c_vec2& operator+=( float fl ) {
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	c_vec2& operator/=( float fl ) {
		x /= fl;
		y /= fl;
		return *this;
	}
	c_vec2& operator-=( float fl ) {
		x -= fl;
		y -= fl;
		return *this;
	}

	// negate the vector components
	void negate( );

	// Get the vector's magnitude.
	[[nodiscard]] vec_t length( ) const;

	// Get the vector's magnitude squared.
	[[nodiscard]] vec_t length_sqr( void ) const {
		return (x * x + y * y);
	}

	// return true if this vector is (0,0,0) within tolerance
	[[nodiscard]] bool is_zero( float tolerance = 0.01f ) const {
		return (x > -tolerance && x < tolerance&&
				 y > -tolerance && y < tolerance);
	}

	vec_t normalize_in_place( );
	[[nodiscard]] c_vec2 normalized( ) const;
	[[nodiscard]] bool is_length_greater_than( float val ) const;
	[[nodiscard]] bool is_length_less_than( float val ) const;

	// check if a vector is within the box defined by two other vectors
	bool within_aa_box( c_vec2 const& boxmin, c_vec2 const& boxmax );

	// Get the distance from this vector to the other one.
	[[nodiscard]] vec_t dist_to( const c_vec2& vOther ) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	[[nodiscard]] vec_t dist_to_sqr( const c_vec2& vOther ) const {
		c_vec2 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.length_sqr( );
	}

	// Copy
	void copy_to_array( float* rgfl ) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void mul_add( const c_vec2& a, const c_vec2& b, float scalar );

	// Dot product.
	[[nodiscard]] vec_t dot( const c_vec2& other ) const;

	// assignment
	c_vec2& operator=( const c_vec2& other );

	// 2d
	[[nodiscard]] vec_t length_2d( void ) const;
	[[nodiscard]] vec_t length_2d_sqr( void ) const;

	/// Get the component of this vector parallel to some other given vector
	c_vec2  project_onto( const c_vec2& onto );

	// copy constructors
	// Vector2D(const Vector2D &vOther);

	// arithmetic operations
	c_vec2 operator-( void ) const;

	c_vec2 operator+( const c_vec2& v ) const;
	c_vec2 operator-( const c_vec2& v ) const;
	c_vec2 operator*( const c_vec2& v ) const;
	c_vec2 operator/( const c_vec2& v ) const;
	c_vec2 operator*( float fl ) const;
	c_vec2 operator/( float fl ) const;

	// Cross product between two vectors.
	[[nodiscard]] c_vec2 Cross( const c_vec2& vOther ) const;

	// Returns a vector with the min or max in X, Y, and Z.
	[[nodiscard]] c_vec2 Min( const c_vec2& other ) const;
	[[nodiscard]] c_vec2 Max( const c_vec2& other ) const;
};