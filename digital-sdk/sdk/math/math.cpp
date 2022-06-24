#include "math.h"
#include "../sdk.hpp"

void c_math::vector_transform(const vec3_t& in1, const matrix_t& in2, vec3_t& out) {
	out[0] = in1.dot(in2[0]) + in2[0][3];
	out[1] = in1.dot(in2[1]) + in2[1][3];
	out[2] = in1.dot(in2[2]) + in2[2][3];
}

float c_math::normalize_yaw(float yaw)
{
	while (yaw > 180)
		yaw -= 360.f;

	while (yaw < -180)
		yaw += 360.f;

	return yaw;
}

float c_math::angle_diff(const float dest_angle, const float src_angle) {
	float delta = fmodf(dest_angle - src_angle, 360.0f);
	if (dest_angle > src_angle) {
		if (delta >= 180)
			delta -= 360;
	}
	else {
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}

qangle_t c_math::calc_angle(const vec3_t& src, const vec3_t& dst) const
{
	qangle_t vAngle;
	const vec3_t delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
	const double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	vAngle.x = atanf(static_cast<float>(delta.z / hyp)) * 57.295779513082f;
	vAngle.y = atanf(delta.y / delta.x) * 57.295779513082f;
	vAngle.z = 0.0f;

	if (delta.x >= 0.0)
		vAngle.y += 180.0f;

	return vAngle;
}

vec3_t cross_product(const vec3_t& a, const vec3_t& b)
{
	return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

void c_math::vector_angles(const vec3_t& forward, qangle_t& view)
{
	float pitch, yaw;

	if (forward.x == 0.f && forward.y == 0.f)
	{
		pitch = (forward.z > 0.f) ? 270.f : 90.f;
		yaw = 0.f;
	}
	else
	{
		pitch = std::atan2f(-forward.z, forward.length_2d()) * 180.f / M_PI;

		if (pitch < 0.f)
			pitch += 360.f;

		yaw = std::atan2f(forward.y, forward.x) * 180.f / M_PI;

		if (yaw < 0.f)
			yaw += 360.f;
	}

	view.x = pitch;
	view.y = yaw;
	view.z = 0.f;
}


void c_math::vector_angles(const vec3_t& forward, const vec3_t& up, qangle_t& angles)
{
	vec3_t left = cross_product(up, forward);
	left.normalize_in_place();

	const float forward_dist = forward.length_2d();

	if (forward_dist > 0.001f)
	{
		angles.x = atan2f(-forward.z, forward_dist) * 180.0f / PI_F;
		angles.y = atan2f(forward.y, forward.x) * 180.0f / PI_F;

		const float up_z = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f(left.z, up_z) * 180.0f / PI_F;
	}
	else
	{
		angles.x = atan2f(-forward.z, forward_dist) * 180.0f / PI_F;
		angles.y = atan2f(-left.x, left.y) * 180.0f / PI_F;
		angles.z = 0;
	}
}

void c_math::sin_cos(const float radians, float* sine, float* cosine) const
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void c_math::angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) const
{
	float sr, sp, sy, cr, cp, cy;

	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

bool c_math::screen_transform(const vec3_t& in, vec3_t& out)
{
	static auto& w2s_matrix = g_sdk.m_interfaces.m_engine->world_to_screen_matrix();

	out.x = w2s_matrix.m[0][0] * in.x + w2s_matrix.m[0][1] * in.y + w2s_matrix.m[0][2] * in.z + w2s_matrix.m[0][3];
	out.y = w2s_matrix.m[1][0] * in.x + w2s_matrix.m[1][1] * in.y + w2s_matrix.m[1][2] * in.z + w2s_matrix.m[1][3];
	out.z = 0.0f;

	const float w = w2s_matrix.m[3][0] * in.x + w2s_matrix.m[3][1] * in.y + w2s_matrix.m[3][2] * in.z + w2s_matrix.m[3][3];

	if (w < 0.001f) {
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	out.x /= w;
	out.y /= w;

	return true;
}

#include "../../utils.h"
bool c_math::world_to_screen(const vec3_t& in, vec3_t& out)
{
	return !g_sdk.m_interfaces.m_debug_overlay->screen_position(in, out);
}