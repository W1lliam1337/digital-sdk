#pragma once

class c_vec3;
class c_move_data;
class c_base_player;

class i_game_movement
{
public:
	//virtual ~c_game_movement(void)
	//= default;
	virtual ~i_game_movement(void)
	{
	}

	virtual void process_movement(c_base_player* player, c_move_data* move) = 0;
	virtual void reset(void) = 0;
	virtual void start_track_prediction_errors(c_base_player* player) = 0;
	virtual void finish_track_prediction_errors(c_base_player* player) = 0;
	virtual void diff_print(const char* fmt, ...) = 0;
	virtual const c_vec3& get_player_mins(bool ducked) const = 0;
	virtual const c_vec3& get_player_maxs(bool ducked) const = 0;
	virtual const c_vec3& get_player_view_offset(bool ducked) const = 0;
	virtual bool is_moving_player_stuck(void) const = 0;
	virtual c_base_player* get_moving_player(void) const = 0;
	virtual void unblock_pusher(c_base_player* player, c_base_player* pusher) = 0;
	virtual void setup_movement_bounds(c_move_data* move) = 0;
};