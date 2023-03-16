#ifndef VALVE_GAME_MOVEMENT
#define VALVE_GAME_MOVEMENT

#include "../../../utils/math/vector.hh"

class c_player;

class c_move_data {
public:
    bool m_first_run_of_functions{ };
    bool m_game_code_moved_player{ };
    int m_player_handle{ };
    int m_impulse_command{ };
    vec3_t m_view_angles{ };
    vec3_t m_abs_view_angles{ };
    int m_buttons{ };
    int m_old_buttons{ };
    float m_forward_move{ };
    float m_side_move{ };
    float m_up_move{ };
    float m_max_speed{ };
    float m_client_max_speed{ };
    vec3_t m_velocity{ };
    vec3_t m_angles{ };
    vec3_t m_old_angles{ };
    float m_out_step_height{ };
    vec3_t m_out_wish_vel{ };
    vec3_t m_out_jump_vel{ };
    vec3_t m_constraint_center{ };
    float m_constraint_radius{ };
    float m_constraint_width{ };
    float m_constraint_speed_factor{ };
private:
    float unknown[ 5 ]{ };
public:
    vec3_t m_abs_origin{ };
};

class i_game_movement {
public:
    virtual ~i_game_movement( void ) {}

    virtual void process_movement( void* player, c_move_data* move ) = 0;
    virtual void reset( void ) = 0;
    virtual void start_track_prediction_errors( void* player ) = 0;
    virtual void finish_track_prediction_errors( void* player ) = 0;
    virtual void diff_print( const char* fmt, ... ) = 0;
    virtual const vec3_t& get_player_mins( bool ducked ) const = 0;
    virtual const vec3_t& get_player_maxs( bool ducked ) const = 0;
    virtual const vec3_t& get_player_view_offset( bool ducked ) const = 0;
    virtual bool is_moving_player_stuck( void ) const = 0;
    virtual c_player* get_moving_player( void ) const = 0;
    virtual void unblock_pusher( void* player, void* pusher ) = 0;
    virtual void setup_movement_bounds( c_move_data* move ) = 0;
};

#endif