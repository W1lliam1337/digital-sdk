#pragma once
#include "../../../sdk/sdk.h"
#include "../../../includes.h"

namespace legit_bot
{
	inline [[nodiscard]] bool crosshair_on_enemy(const qangle_t view_angles)
	{
		const auto& start_pos = ctx::local()->get_shoot_pos();
		c_vec3 direction;
		math::angle_vectors(view_angles + ctx::local()->get_aim_punch_angle(), &direction);

		const auto end_pos = start_pos + direction * 8096;

		c_game_trace exit_trace{};
		c_trace_filter filter;
		filter.m_skip = ctx::local();

		ray_t ray;
		ray.init(start_pos, end_pos);

		interfaces::m_trace->trace_ray(ray, mask_shot | contents_grate, &filter, &exit_trace);

		return exit_trace.m_entity && exit_trace.m_entity->get_team() != ctx::local()->get_team();
	}

	void init();
	void rcs();
	void aimbot(c_weapon* active_weapon);
};
