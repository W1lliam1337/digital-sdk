#ifndef VALVE_PREDICTION
#define VALVE_PREDICTION

#include "../../../utils/math/math.hh"
#include "../../modules/modules.hh"
#include "globals.hh"

class i_move_helper;
class c_user_cmd;
class c_player;

class i_prediction {
    [[maybe_unused]] std::byte pad0[ 8 ]{ };
public:
    bool m_in_prediction{ };
private:
    [[maybe_unused]] std::byte pad1[ 1 ]{ };
public:
    bool m_engine_paused{ };
    bool m_old_cl_predict_value{ };
    int m_prev_start_frame{ };
    int m_incoming_packet_number{ };
    float m_last_server_world_time_stamp{ };
    bool m_first_time_predicted{ };
private:
    [[maybe_unused]] std::byte pad2[ 3 ]{ };
public:
    int m_cmds_predicted{ };
    int m_server_cmds_acknowledged{ };
    bool m_prev_ack_had_errors{ };
private:
    [[maybe_unused]] std::byte pad3[ 4 ]{ };
public:
    int m_incoming_acknowledged{ };
private:
    [[maybe_unused]] std::byte pad4[ 4 ]{ };
public:
    c_utl_vector< void* > m_prediction_handles{ };
private:
    [[maybe_unused]] std::byte pad5[ 4 ]{ };
public:
    i_global_vars m_saved_globals{ };

    VFUNC( void( __thiscall* )( void*, int, bool, int, int ),
           update( const int server_tick, const bool is_valid, const int acknow_ledged, const int outgoing_cmd ), 3,
           server_tick, is_valid, acknow_ledged, outgoing_cmd );

    VFUNC( void( __thiscall* )( void*, vec3_t& ), set_local_view_angles( vec3_t& angles ), 13, angles );

    VFUNC( void( __thiscall* )( void*, c_player*, double ),
           check_moving_ground( c_player* player, const double frame_time ), 18, player, frame_time );

    VFUNC( void( __thiscall* )( void*, c_player*, c_user_cmd*, i_move_helper*, void* ),
           setup_move( c_player* player, c_user_cmd* cmd, i_move_helper* move_helper, void* move_data ), 20, player,
           cmd, move_helper, move_data );

    VFUNC( void( __thiscall* )( void*, c_player*, c_user_cmd*, void* ),
           finish_move( c_player* player, c_user_cmd* cmd, void* move_data ), 21, player, cmd, move_data );

    void restore_entity_to_predicted_frame( const int frame ) const {
        static const auto sig = g_modules->m_client_dll.get_address( _( "CPrediction::RestoreEntityToPredFrame" ) ).as< 
            void( __stdcall* )( int, int ) >( );
        return sig( 0, frame );
    }

    void repredict( ) {
        this->restore_entity_to_predicted_frame( this->m_prev_start_frame - 1 );
        this->m_prev_start_frame = -1;
        this->m_cmds_predicted = 0;
    }
};

#endif