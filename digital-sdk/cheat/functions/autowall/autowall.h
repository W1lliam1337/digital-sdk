#pragma once
#include "../../../sdk/sdk.hpp"
#include "../../../dependencies/utils.h"

struct fire_bullet_data_t
{
	float m_current_damage{};
	float m_current_distance{};

	int	m_penetrate_count{};

	vec3_t m_position{};
	vec3_t m_direction{};

	c_game_trace m_enter_trace{};
	c_game_trace m_exit_trace{};

	c_weapon* m_weapon{};
	c_weapon_info* m_weapon_info{};
};

class c_autowall : public c_singleton<c_autowall>
{
public:
	void scale_damage(fire_bullet_data_t& data, float& damage) const;
	bool trace_to_exit(c_game_trace* enter_trace, c_game_trace* exit_trace, const vec3_t& pos, vec3_t& dir) const;
	bool handle_bullet_penetration(fire_bullet_data_t& data) const;
	bool simulate_fire_bullet(fire_bullet_data_t& data) const;
	float get_damage(const vec3_t& point, fire_bullet_data_t* data) const;
	static void clip_trace_to_players(const vec3_t& abs_start, const vec3_t& abs_end, unsigned mask, i_trace_filter* filter, c_game_trace* trace);
};

