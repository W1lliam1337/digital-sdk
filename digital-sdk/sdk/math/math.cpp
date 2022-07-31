#include "math.h"

void math::vector_transform(const c_vec3& in1, const matrix_t& in2, c_vec3& out) {
	out[0] = in1.dot(in2[0]) + in2[0][3];
	out[1] = in1.dot(in2[1]) + in2[1][3];
	out[2] = in1.dot(in2[2]) + in2[2][3];
}

float math::normalize_yaw(float yaw)
{
	while (yaw > 180)
		yaw -= 360.f;

	while (yaw < -180)
		yaw += 360.f;

	return yaw;
}

float math::angle_diff(const float dest_angle, const float src_angle) {
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

qangle_t math::calc_angle(const c_vec3& src, const c_vec3& dst) {
	qangle_t angle;
	const c_vec3 delta(src.x - dst.x, src.y - dst.y, src.z - dst.z);
	const double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = atanf(static_cast<float>(delta.z / hyp)) * 57.295779513082f;
	angle.y = atanf(delta.y / delta.x) * 57.295779513082f;
	angle.z = 0.0f;

	if (delta.x >= 0.0)
		angle.y += 180.0f;

	return angle;
}

c_vec3 cross_product(const c_vec3& a, const c_vec3& b)
{
	return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

void math::vector_angles(const c_vec3& forward, qangle_t& view)
{
	float pitch, yaw;

	if (forward.x == 0.f && forward.y == 0.f)
	{
		pitch = forward.z > 0.f ? 270.f : 90.f;
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


void math::vector_angles(const c_vec3& forward, const c_vec3& up, qangle_t& angles)
{
	c_vec3 left = cross_product(up, forward);
	left.normalize_in_place();

	const float forward_dist = forward.length_2d();

	if (forward_dist > 0.001f)
	{
		angles.x = atan2f(-forward.z, forward_dist) * 180.0f / PI_F;
		angles.y = atan2f(forward.y, forward.x) * 180.0f / PI_F;

		const float up_z = left.y * forward.x - left.x * forward.y;
		angles.z = atan2f(left.z, up_z) * 180.0f / PI_F;
	}
	else
	{
		angles.x = atan2f(-forward.z, forward_dist) * 180.0f / PI_F;
		angles.y = atan2f(-left.x, left.y) * 180.0f / PI_F;
		angles.z = 0;
	}
}

void math::sin_cos(const float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void math::angle_vectors(const qangle_t& angles, c_vec3* forward, c_vec3* right, c_vec3* up)
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
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

qangle_t math::angle_normalize(qangle_t &angle)
{
	while (angle.x > 89.f) {
		angle.x -= 180.f;
	}
	while (angle.x < -89.f) {
		angle.x += 180.f;
	}
	if (angle.y > 180) {
		angle.y -= round(angle.y / 360) * 360.f;
	}
	else if (angle.y < -180) {
		angle.y += round(angle.y / 360) * -360.f;
	}
	if (angle.z > 50 || angle.z < 50) {
		angle.z = 0;
	}
	return angle;
}

bool math::screen_transform(const c_vec3& in, c_vec3& out)
{
	static auto& w2s_matrix = interfaces::m_engine->world_to_screen_matrix();

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

int math::random_int(const int min, const int max)
{
	static auto dw_address = reinterpret_cast<DWORD>(GetProcAddress(modules::m_vstd_dll, _("RandomInt")));
	return reinterpret_cast<int(*)(int, int)>(dw_address)(min, max);
}

float math::random_float(const float min, const float max)
{
	static auto dw_address = reinterpret_cast<DWORD>(GetProcAddress(modules::m_vstd_dll, _("RandomFloat")));
	return reinterpret_cast<float(*)(float, float)>(dw_address)(min, max);
}

void math::random_seed(const int seed)
{
	static auto dw_address = reinterpret_cast<DWORD>(GetProcAddress(modules::m_vstd_dll, _("RandomSeed")));
	return reinterpret_cast<void(*)(int)>(dw_address)(seed);
}
