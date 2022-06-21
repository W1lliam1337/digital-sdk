#pragma once

using vec_t = float;

// 2D Vector
class vector_2d
{
public:
	// Members
	vec_t x, y;

	// Construction/destruction:
	vector_2d(void);
	vector_2d(vec_t X, vec_t Y);
	vector_2d(vec_t* clr);

	vector_2d(const vector_2d& vOther)
	{
		x = vOther.x;
		y = vOther.y;
	}

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	const vec_t* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

	// equality
	bool operator==(const vector_2d& v) const;
	bool operator!=(const vector_2d& v) const;

	// arithmetic operations
	vector_2d& operator+=(const vector_2d& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	vector_2d& operator-=(const vector_2d& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vector_2d& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}

	vector_2d& operator*=(const vector_2d& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	vector_2d& operator/=(const vector_2d& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	// this ought to be an opcode.
	vector_2d& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	vector_2d& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}

	vector_2d& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	vec_t Length() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const
	{
		return (x * x + y * y);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance);
	}

	vec_t NormalizeInPlace();
	vector_2d Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(const vector_2d& boxmin, const vector_2d& boxmax);

	// Get the distance from this vector to the other one.
	vec_t DistTo(const vector_2d& vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const vector_2d& vOther) const
	{
		vector_2d delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const vector_2d& a, const vector_2d& b, float scalar);

	// Dot product.
	vec_t Dot(const vector_2d& vOther) const;

	// assignment
	vector_2d& operator=(const vector_2d& vOther);

	// 2d
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	vector_2d ProjectOnto(const vector_2d& onto);

	// copy constructors
	// Vector2D(const Vector2D &vOther);

	// arithmetic operations
	vector_2d operator-(void) const;

	vector_2d operator+(const vector_2d& v) const;
	vector_2d operator-(const vector_2d& v) const;
	vector_2d operator*(const vector_2d& v) const;
	vector_2d operator/(const vector_2d& v) const;
	vector_2d operator*(float fl) const;
	vector_2d operator/(float fl) const;

	// Cross product between two vectors.
	vector_2d Cross(const vector_2d& vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	vector_2d Min(const vector_2d& vOther) const;
	vector_2d Max(const vector_2d& vOther) const;
};
