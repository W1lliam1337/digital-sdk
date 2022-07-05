#pragma once
#include "../../sdk/sdk.h"
#include "../../includes.h"

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

class c_auto_wall
{
public:
	void scale_damage(float& damage) const;
	bool trace_to_exit(c_game_trace* enter_trace, c_game_trace* exit_trace, vec3_t start, vec3_t& end);
	bool handle_bullet_penetration();
	bool simulate_fire_bullet();
	float get_damage(const vec3_t& point);
	static void clip_trace_to_players(const vec3_t& abs_start, const vec3_t& abs_end, c_trace_filter* filter, c_game_trace* trace, float min_range = 0.0f);
	fire_bullet_data_t m_fire_bullet_data{};
};

inline c_auto_wall g_auto_wall;