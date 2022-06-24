#pragma once
#include <cfloat>
#include <cmath>

class vec3_t
{
public:
	vec3_t(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	vec3_t()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	};

	vec3_t(float fx, float fy, float fz)
	{
		this->x = fx;
		this->y = fy;
		this->z = fz;
	}

	void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix;
		y = iy;
		z = iz;
	}

	vec3_t operator*(float fl)
	{
		return { x * fl, y * fl, z * fl };
	}

	vec3_t operator*(const vec3_t& v)
	{
		return { x * v.x, y * v.y, z * v.z };
	}

	__inline void mul(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	float& operator[](const int i)
	{
		return reinterpret_cast<float*>(this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	[[nodiscard]] bool __inline is_zero() const
	{
		return !x && !y && !z;
	}

	[[nodiscard]] bool is_valid() const
	{
		return isfinite(x) && isfinite(y) && isfinite(z) && x < 9999999 && y < 9999999 && z < 9999999;
	}

	vec3_t clamp()
	{
		if (x < -89.0f)
		{
			x = -89.0f;
		}

		if (x > 89.0f)
		{
			x = 89.0f;
		}

		while (y < -180.0f)
		{
			y += 360.0f;
		}

		while (y > 180.0f)
		{
			y -= 360.0f;
		}

		z = 0.0f;
		return *this;
	}

	void __inline zero()
	{
		x = y = z = 0.0f;
	}

	bool operator==(const vec3_t& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool operator!=(const vec3_t& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	vec3_t& operator+=(const vec3_t& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3_t& operator-=(const vec3_t& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	vec3_t& operator*=(const float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	vec3_t& operator*=(const vec3_t& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	vec3_t& operator/=(const vec3_t& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	vec3_t& operator+=(const float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	vec3_t& operator/=(const float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	vec3_t& operator-=(const float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void normalize_in_place()
	{
		*this = normalized();
	}

	vec3_t normalized()
	{
		vec3_t res = *this;
		if (const float l = res.length(); l != 0.0f)
		{
			res /= l;
		}
		else
		{
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	[[nodiscard]] float normalize() const
	{
		vec3_t res = *this;
		const float l = res.length();
		if (l != 0.0f)
		{
			res /= l;
		}
		else
		{
			res.x = res.y = res.z = 0.0f;
		}
		return l;
	}

	[[nodiscard]] float dist_to(const vec3_t& v_other) const
	{
		vec3_t delta;

		delta.x = x - v_other.x;
		delta.y = y - v_other.y;
		delta.z = z - v_other.z;

		return delta.length();
	}

	[[nodiscard]] float dist_to_sqr(const vec3_t& v_other) const
	{
		vec3_t delta;

		delta.x = x - v_other.x;
		delta.y = y - v_other.y;
		delta.z = z - v_other.z;

		return delta.length_sqr();
	}

	[[nodiscard]] float dot(const vec3_t& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}

	static void vector_cross_product(const vec3_t& a, const vec3_t& b, vec3_t& result)
	{
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}

	vec3_t cross(const vec3_t& v_other) const
	{
		vec3_t res;
		vector_cross_product(*this, v_other, res);
		return res;
	}

	[[nodiscard]] float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	[[nodiscard]] float length_sqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	[[nodiscard]] float length_2d_sqr() const
	{
		return (x * x + y * y);
	}

	[[nodiscard]] float length_2d() const
	{
		return sqrt(x * x + y * y);
	}

	vec3_t& operator=(const vec3_t& vOther)
	{
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;
		return *this;
	}

	vec3_t operator-() const
	{
		return { -x, -y, -z };
	}

	vec3_t operator+(const vec3_t& v) const
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	vec3_t operator+(float fl) const
	{
		return { x + fl, y + fl, z + fl };
	}

	vec3_t operator-(const vec3_t& v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	vec3_t operator-(float fl) const
	{
		return { x - fl, y - fl, z - fl };
	}

	vec3_t operator*(float fl) const
	{
		return { x * fl, y * fl, z * fl };
	}

	vec3_t operator*(const vec3_t& v) const
	{
		return { x * v.x, y * v.y, z * v.z };
	}

	vec3_t operator/(float fl) const
	{
		return { x / fl, y / fl, z / fl };
	}

	vec3_t operator/(const vec3_t& v) const
	{
		return { x / v.x, y / v.y, z / v.z };
	}

	float x;
	float y;
	float z;
};
