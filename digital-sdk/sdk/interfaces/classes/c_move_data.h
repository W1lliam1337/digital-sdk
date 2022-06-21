#pragma once

class c_move_data
{
public:
	bool            m_first_run_of_functions : 1;
	bool            m_game_code_moved_player : 1;
	bool            m_no_air_control : 1;
	int	m_player_handle{};
	int				m_impulse_command{};
	qangle_t		m_view_angles{};
	qangle_t		m_abs_view_angles{};
	int				m_buttons{};
	int				m_old_buttons{};
	vec3_t			m_move{};
	float			m_max_speed{};
	float			m_client_max_speed{};
	vec3_t			m_velocity{};
	vec3_t			m_trailing_velocity{};
	float				m_trailing_velocity_time{};
	qangle_t        m_angles{};
	qangle_t		m_old_angles{};
	float			m_out_step_height{};
	vec3_t			m_out_wish_velocity{};
	vec3_t			m_out_jump_velocity{};
	vec3_t			m_constraint_center{};
	float			m_constraint_radius{};
	float			m_constraint_width{};
	float			m_constraint_speed_factor{};
	bool            m_constraint_past_radius{};
	vec3_t			m_abs_origin{};
};
