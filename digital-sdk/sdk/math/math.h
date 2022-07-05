#pragma once
#include "matrix.h"
#include "qangle.h"
#include "vec3.h"
#include <corecrt_math_defines.h>
#include "../interfaces/interfaces.h"

#define PI_F ( ( float )( M_PI ) ) 
#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define squares( x ) ( x * x )

class c_math 
{
public:
	static void vector_transform(const vec3_t& in1, const matrix_t& in2, vec3_t& out);
	static float normalize_yaw(float yaw);
	static float angle_diff(const float dest_angle, const float src_angle);
	[[nodiscard]] qangle_t calc_angle(const vec3_t& src, const vec3_t& dst) const;
	static void vector_angles(const vec3_t& forward, qangle_t& view);
	static void vector_angles(const vec3_t& forward, const vec3_t& up, qangle_t& angles);
	void sin_cos(float radians, float* sine, float* cosine) const;
	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr) const;
	qangle_t angle_normalize(qangle_t& angle) const;
	static bool screen_transform(const vec3_t& in, vec3_t& out);
};

inline c_math g_math;