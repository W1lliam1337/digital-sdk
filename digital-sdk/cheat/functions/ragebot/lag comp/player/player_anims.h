#pragma once
#include "../../../../../sdk/sdk.hpp"
#include <deque>

enum e_matrices
{
	main_matrix,
	left_matrix,
	right_matrix,
	matrices_count
};

class c_lag_record
{
public:
	float m_duck_amount{};
	float m_simulation_time{};
	float m_lower_body_yaw{};

	bit_flag_t<int> m_flags{};
	int m_choked_ticks{};

	vec3_t m_velocity{};
	vec3_t m_origin{};
	vec3_t m_abs_origin{};
	vec3_t m_mins{};
	vec3_t m_maxs{};
	qangle_t m_eye_angles{};

	std::array<animlayer_t, animation_layer_count> m_animation_layers{};
	std::array<std::array<matrix_t, 128>, matrices_count> m_matrices{};
	std::array<float, 24> m_pose_parameters{};
};

class c_player_lagcomp : public c_singleton<c_player_lagcomp>
{
	//std::deque < c_lag_record > m_previous_records[65]{};
	c_lag_record* m_previous_record{};
	std::deque < c_lag_record> m_current_records[65]{};
public:
	void on_frame();
	void update_player(c_base_player* player);
	static void fill_record(c_base_player* player, c_lag_record& lag_record);
	static void build_matrix(c_base_player* player, matrix_t* matrix, const int mask);
};