#pragma once
#include "../../sdk/sdk.h"
#include "../../includes.h"

class c_legit_bot
{
	[[nodiscard]] bool crosshair_on_enemy(const qangle_t view_angles) const
	{
		const auto& start_pos = g_sdk.m_local()->get_shoot_pos();
		vec3_t direction;
		g_math.angle_vectors(view_angles + g_sdk.m_local()->get_aim_punch_angle(), &direction);

		const auto end_pos = start_pos + direction * 8096;

		c_game_trace exit_trace{};
		c_trace_filter filter;
		filter.m_skip = g_sdk.m_local();

		ray_t ray;
		ray.init(start_pos, end_pos);

		g_interfaces.m_trace->trace_ray(ray, mask_shot | contents_grate, &filter, &exit_trace);

		return exit_trace.m_entity && exit_trace.m_entity->get_team() != g_sdk.m_local()->get_team();
	}
public:
	void init();
	static void rcs();
	void aimbot(c_weapon* active_weapon);
};

inline c_legit_bot g_legit_bot;
