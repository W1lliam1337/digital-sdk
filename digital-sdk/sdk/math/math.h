#pragma once
#include "../../dependencies/singleton/singleton.h"
#include "vec3_t.h"
#include "vec2_t.h"
#include "vec4_t.h"
#include "qangle.h"
#include "vmatrix.h"

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define squares( x ) ( x * x )

#define M_PI 3.14159265358979323846f
#define PI_F ( ( float )( M_PI ) ) 
#define M_RADPI 57.295779513082f

class c_math : public c_singleton<c_math>
{
public:
	static void vector_transform(const vec3_t& in1, const matrix_t& in2, vec3_t& out);
	static float normalize_yaw(float yaw);
	static float angle_diff(const float dest_angle, const float src_angle);
	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst) const;
	static void vector_angles(const vec3_t& forward, qangle_t& view);
	static void vector_angles(const vec3_t& forward, const vec3_t& up, qangle_t& angles);
	void sin_cos(float radians, float* sine, float* cosine) const;
	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr) const;
	static bool screen_transform(const vec3_t& in, vec3_t& out);
	static bool world_to_screen(const vec3_t& in, vec3_t& out);
};

