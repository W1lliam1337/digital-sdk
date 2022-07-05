#include "vec2.h"
#include <cmath>

vec2_t::vec2_t(void)
{
}

vec2_t::vec2_t(vec_t X, vec_t Y)
{
	x = X; y = Y;
}

vec2_t::vec2_t(vec_t* clr)
{
	x = clr[0]; y = clr[1];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void vec2_t::Init(vec_t ix, vec_t iy)
{
	x = ix; y = iy;
}

void vec2_t::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

void Vector2DClear(vec2_t& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

vec2_t& vec2_t::operator=(const vec2_t& vOther)
{
	x = vOther.x; y = vOther.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

vec_t& vec2_t::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t vec2_t::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

vec_t* vec2_t::Base()
{
	return (vec_t*)this;
}

vec_t const* vec2_t::Base() const
{
	return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool vec2_t::IsValid() const
{
	return !isinf(x) && !isinf(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool vec2_t::operator==(const vec2_t& src) const
{
	return (src.x == x) && (src.y == y);
}

bool vec2_t::operator!=(const vec2_t& src) const
{
	return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

void Vector2DCopy(const vec2_t& src, vec2_t& dst)
{
	dst.x = src.x;
	dst.y = src.y;
}

void vec2_t::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void vec2_t::Negate()
{
	x = -x; y = -y;
}

void Vector2DAdd(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

void Vector2DSubtract(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

void Vector2DMultiply(const vec2_t& a, vec_t b, vec2_t& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

void Vector2DMultiply(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}


void Vector2DDivide(const vec2_t& a, vec_t b, vec2_t& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

void Vector2DDivide(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

void Vector2DMA(const vec2_t& start, float s, const vec2_t& dir, vec2_t& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
void vec2_t::MulAdd(const vec2_t& a, const vec2_t& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

void Vector2DLerp(const vec2_t& src1, const vec2_t& src2, vec_t t, vec2_t& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
vec_t DotProduct2D(const vec2_t& a, const vec2_t& b)
{
	return(a.x * b.x + a.y * b.y);
}

// for backwards compatability
vec_t vec2_t::Dot(const vec2_t& vOther) const
{
	return DotProduct2D(*this, vOther);
}

vec_t Vector2DNormalize(vec2_t& v)
{
	vec_t l = v.Length();
	if (l != 0.0f) {
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
vec_t Vector2DLength(const vec2_t& v)
{
	return (vec_t)sqrt(v.x * v.x + v.y * v.y);
}

vec_t vec2_t::NormalizeInPlace()
{
	return Vector2DNormalize(*this);
}

bool vec2_t::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val * val;
}

bool vec2_t::IsLengthLessThan(float val) const
{
	return LengthSqr() < val * val;
}

vec_t vec2_t::Length(void) const
{
	return Vector2DLength(*this);
}


void Vector2DMin(const vec2_t& a, const vec2_t& b, vec2_t& result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}


void Vector2DMax(const vec2_t& a, const vec2_t& b, vec2_t& result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}

//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
void ComputeClosestPoint2D(const vec2_t& vecStart, float flMaxDist, const vec2_t& vecTarget, vec2_t* pResult)
{
	vec2_t vecDelta;
	Vector2DSubtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist) {
		*pResult = vecTarget;
	}
	else {
		vecDelta /= sqrt(flDistSqr);
		Vector2DMA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

vec2_t vec2_t::Min(const vec2_t& vOther) const
{
	return vec2_t(x < vOther.x ? x : vOther.x, y < vOther.y ? y : vOther.y);
}

vec2_t vec2_t::Max(const vec2_t& vOther) const
{
	return vec2_t(x > vOther.x ? x : vOther.x, y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

vec2_t vec2_t::operator-(void) const
{
	return vec2_t(-x, -y);
}

vec2_t vec2_t::operator+(const vec2_t& v) const
{
	vec2_t res;
	Vector2DAdd(*this, v, res);
	return res;
}

vec2_t vec2_t::operator-(const vec2_t& v) const
{
	vec2_t res;
	Vector2DSubtract(*this, v, res);
	return res;
}

vec2_t vec2_t::operator*(float fl) const
{
	vec2_t res;
	Vector2DMultiply(*this, fl, res);
	return res;
}

vec2_t vec2_t::operator*(const vec2_t& v) const
{
	vec2_t res;
	Vector2DMultiply(*this, v, res);
	return res;
}

vec2_t vec2_t::operator/(float fl) const
{
	vec2_t res;
	Vector2DDivide(*this, fl, res);
	return res;
}

vec2_t vec2_t::operator/(const vec2_t& v) const
{
	vec2_t res;
	Vector2DDivide(*this, v, res);
	return res;
}

vec2_t operator*(float fl, const vec2_t& v)
{
	return v * fl;
}