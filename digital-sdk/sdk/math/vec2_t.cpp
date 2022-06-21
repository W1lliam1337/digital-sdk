#include "vec2_t.h"
#include <cmath>

vector_2d::vector_2d(void)
{
}

vector_2d::vector_2d(vec_t X, vec_t Y)
{
	x = X;
	y = Y;
}

vector_2d::vector_2d(vec_t* clr)
{
	x = clr[0];
	y = clr[1];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void vector_2d::Init(vec_t ix, vec_t iy)
{
	x = ix;
	y = iy;
}

void vector_2d::Random(float minVal, float maxVal)
{
	x = minVal + (static_cast<float>(rand()) / RAND_MAX) * (maxVal - minVal);
	y = minVal + (static_cast<float>(rand()) / RAND_MAX) * (maxVal - minVal);
}

void Vector2DClear(vector_2d& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

vector_2d& vector_2d::operator=(const vector_2d& vOther)
{
	x = vOther.x;
	y = vOther.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

vec_t& vector_2d::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t vector_2d::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

vec_t* vector_2d::Base()
{
	return (vec_t*)this;
}

const vec_t* vector_2d::Base() const
{
	return (const vec_t*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool vector_2d::IsValid() const
{
	return !isinf(x) && !isinf(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool vector_2d::operator==(const vector_2d& src) const
{
	return (src.x == x) && (src.y == y);
}

bool vector_2d::operator!=(const vector_2d& src) const
{
	return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

void Vector2DCopy(const vector_2d& src, vector_2d& dst)
{
	dst.x = src.x;
	dst.y = src.y;
}

void vector_2d::CopyToArray(float* rgfl) const
{
	rgfl[0] = x;
	rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void vector_2d::Negate()
{
	x = -x;
	y = -y;
}

void Vector2DAdd(const vector_2d& a, const vector_2d& b, vector_2d& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

void Vector2DSubtract(const vector_2d& a, const vector_2d& b, vector_2d& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

void Vector2DMultiply(const vector_2d& a, vec_t b, vector_2d& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

void Vector2DMultiply(const vector_2d& a, const vector_2d& b, vector_2d& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}


void Vector2DDivide(const vector_2d& a, vec_t b, vector_2d& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

void Vector2DDivide(const vector_2d& a, const vector_2d& b, vector_2d& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

void Vector2DMA(const vector_2d& start, float s, const vector_2d& dir, vector_2d& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
void vector_2d::MulAdd(const vector_2d& a, const vector_2d& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

void Vector2DLerp(const vector_2d& src1, const vector_2d& src2, vec_t t, vector_2d& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
vec_t DotProduct2D(const vector_2d& a, const vector_2d& b)
{
	return (a.x * b.x + a.y * b.y);
}

// for backwards compatability
vec_t vector_2d::Dot(const vector_2d& vOther) const
{
	return DotProduct2D(*this, vOther);
}

vec_t Vector2DNormalize(vector_2d& v)
{
	vec_t l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f;
	}
	return l;
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
vec_t Vector2DLength(const vector_2d& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

vec_t vector_2d::NormalizeInPlace()
{
	return Vector2DNormalize(*this);
}

bool vector_2d::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val * val;
}

bool vector_2d::IsLengthLessThan(float val) const
{
	return LengthSqr() < val * val;
}

vec_t vector_2d::Length(void) const
{
	return Vector2DLength(*this);
}


void Vector2DMin(const vector_2d& a, const vector_2d& b, vector_2d& result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}


void Vector2DMax(const vector_2d& a, const vector_2d& b, vector_2d& result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}

//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
void ComputeClosestPoint2D(const vector_2d& vecStart, float flMaxDist, const vector_2d& vecTarget, vector_2d* pResult)
{
	vector_2d vecDelta;
	Vector2DSubtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist)
	{
		*pResult = vecTarget;
	}
	else
	{
		vecDelta /= sqrt(flDistSqr);
		Vector2DMA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

vector_2d vector_2d::Min(const vector_2d& vOther) const
{
	return vector_2d(x < vOther.x ? x : vOther.x, y < vOther.y ? y : vOther.y);
}

vector_2d vector_2d::Max(const vector_2d& vOther) const
{
	return vector_2d(x > vOther.x ? x : vOther.x, y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

vector_2d vector_2d::operator-(void) const
{
	return vector_2d(-x, -y);
}

vector_2d vector_2d::operator+(const vector_2d& v) const
{
	vector_2d res;
	Vector2DAdd(*this, v, res);
	return res;
}

vector_2d vector_2d::operator-(const vector_2d& v) const
{
	vector_2d res;
	Vector2DSubtract(*this, v, res);
	return res;
}

vector_2d vector_2d::operator*(float fl) const
{
	vector_2d res;
	Vector2DMultiply(*this, fl, res);
	return res;
}

vector_2d vector_2d::operator*(const vector_2d& v) const
{
	vector_2d res;
	Vector2DMultiply(*this, v, res);
	return res;
}

vector_2d vector_2d::operator/(float fl) const
{
	vector_2d res;
	Vector2DDivide(*this, fl, res);
	return res;
}

vector_2d vector_2d::operator/(const vector_2d& v) const
{
	vector_2d res;
	Vector2DDivide(*this, v, res);
	return res;
}

vector_2d operator*(float fl, const vector_2d& v)
{
	return v * fl;
}
