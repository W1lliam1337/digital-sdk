#pragma once
#include "../../../sdk/sdk.h"
#include "../../../includes.h"

struct fire_bullet_data_t
{
	float m_current_damage{};
	float m_current_distance{};

	int	m_penetrate_count{};

	c_vec3 m_position{};
	c_vec3 m_direction{};

	c_game_trace m_enter_trace{};
	c_game_trace m_exit_trace{};

	c_weapon* m_weapon{};
	c_weapon_info* m_weapon_info{};
};

namespace auto_wall
{
	void scale_damage(float& damage);
	bool trace_to_exit(c_game_trace* enter_trace, c_game_trace* exit_trace, c_vec3 start, c_vec3& end);
	bool handle_bullet_penetration();
	bool simulate_fire_bullet();
	float get_damage(const c_vec3& point);
	void clip_trace_to_players(const c_vec3& abs_start, const c_vec3& abs_end, c_trace_filter* filter, c_game_trace* trace, float min_range = 0.0f);
	inline fire_bullet_data_t m_fire_bullet_data{};
}