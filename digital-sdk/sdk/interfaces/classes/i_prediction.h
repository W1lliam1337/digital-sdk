#pragma once
#include "i_globals.h"
#include "../../other/utl/utl.h"
#include "../../../utils/utils.h"

struct qangle_t;
class c_user_cmd;
class c_base_handle;
class c_base_entity;
class i_move_helper;

class i_prediction
{
public:
	char						pad0[8]{};
	bool						m_in_prediction{};
	char						pad1[1]{};
	bool						m_engine_paused{};
	bool						m_old_cl_predict_value{};
	int							m_prev_start_frame{};
	int							m_incoming_packet_number{};
	float						m_last_server_world_time_stamp{};
	bool						m_first_time_predicted{};
	char						pad2[3]{};
	int							m_commands_predicted{};
	int							m_server_commands_acknowledged{};
	bool						m_prev_ack_had_errors{};
	char						pad3[4]{};
	int							m_incoming_acknowledged{};
	char						pad4[4]{};
	c_utl_vector<c_base_handle>     m_prediction_handles{};
	char						pad5[4]{};
	i_global_vars_base			m_saved_globals{};

	void force_repredict()
	{
		this->m_prev_start_frame = -1;
		this->m_commands_predicted = 0;
	}

	void update(const int server_tick, const bool is_valid, const int acknowledged, const int outgoing_cmd)
	{
		using update_fn = void(__thiscall*)(void*, int, bool, int, int);
		return utils::call_vfunc<update_fn>(this, 3)(this, server_tick, is_valid, acknowledged, outgoing_cmd);
	}

	void setup_move(c_base_entity* player, c_user_cmd* cmd, i_move_helper* move_helper, void* move_data)
	{
		using setup_move_fn = void(__thiscall*)(void*, c_base_entity*, c_user_cmd*, i_move_helper*, void*);
		return utils::call_vfunc<setup_move_fn>(this, 20)(this, player, cmd, move_helper, move_data);
	}

	void finish_move(c_base_entity* player, c_user_cmd* cmd, void* move_data)
	{
		using finish_move_fn = void(__thiscall*)(void*, c_base_entity*, c_user_cmd*, void*);
		return utils::call_vfunc<finish_move_fn>(this, 21)(this, player, cmd, move_data);
	}

	void set_local_view_angles(qangle_t& angles)
	{
		using set_local_view_angles_fn = void(__thiscall*)(void*, qangle_t&);
		return utils::call_vfunc<set_local_view_angles_fn>(this, 13)(this, angles);
	}

	void check_moving_ground(c_base_entity* player, const double frame_time)
	{
		using check_moving_ground_fn = void(__thiscall*)(void*, c_base_entity*, double);
		return utils::call_vfunc<check_moving_ground_fn>(this, 18)(this, player, frame_time);
	}
};