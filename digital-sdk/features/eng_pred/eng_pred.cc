#include "eng_pred.hh"

void c_eng_pred::predict( const std::function< void( ) >& fn ) {
    this->begin( );
    fn( );
    this->end( );
}

void c_eng_pred::begin( ) {
    if ( !c_player::get_local( ) || !c_player::get_local( )->is_alive( ) || !g_ctx->local( ).m_cmd )
        return;

    m_backup_data.store( );

    c_player::get_local( )->current_cmd( ) = g_ctx->local( ).m_cmd;
    c_player::get_local( )->last_cmd( ) = g_ctx->local( ).m_cmd;

    c_player::get_local( )->set_pred_player( c_player::get_local( ) );
    c_player::get_local( )->set_pred_random_seed( g_ctx->local( ).m_cmd );

    g_interfaces->m_globals->m_current_time = to_time( c_player::get_local( )->tick_base( ) );
    g_interfaces->m_globals->m_frame_time =
        g_interfaces->m_prediction->m_engine_paused ? 0.0f : g_interfaces->m_globals->m_interval_per_tick;

    g_ctx->local( ).m_cmd->m_buttons |= c_player::get_local( )->button_forced( );
    g_ctx->local( ).m_cmd->m_buttons &= ~c_player::get_local( )->button_disabled( );

    g_interfaces->m_prediction->m_in_prediction = true;
    g_interfaces->m_prediction->m_first_time_predicted = false;

    g_interfaces->m_game_movement->start_track_prediction_errors( c_player::get_local( ) );
    if ( g_ctx->local( ).m_cmd->m_impulse )
        c_player::get_local( )->impulse( ) = g_ctx->local( ).m_cmd->m_impulse;

	c_player::get_local( )->update_button_state( g_ctx->local( ).m_cmd->m_buttons );

    g_interfaces->m_move_helper->set_host( c_player::get_local( ) );
    g_interfaces->m_prediction->check_moving_ground( c_player::get_local( ), g_interfaces->m_globals->m_frame_time );
    g_interfaces->m_prediction->set_local_view_angles( g_ctx->local( ).m_cmd->m_view_angles );

    if ( c_player::get_local( )->physics_run_think( ) ) {
        c_player::get_local( )->pre_think( );
    }

    if ( c_player::get_local( )->next_think_tick( ) > 0
         || c_player::get_local( )->next_think_tick( ) < c_player::get_local( )->tick_base( ) ) {
        c_player::get_local( )->next_think_tick( ) = -1;
        c_player::get_local( )->unk_think_func( 0 );
        c_player::get_local( )->think( );
    }

    g_interfaces->m_prediction->setup_move( c_player::get_local( ), g_ctx->local( ).m_cmd, g_interfaces->m_move_helper,
                                            g_interfaces->m_move_data );
    g_interfaces->m_game_movement->process_movement( c_player::get_local( ), g_interfaces->m_move_data );
    g_interfaces->m_prediction->finish_move( c_player::get_local( ), g_ctx->local( ).m_cmd, g_interfaces->m_move_data );
    g_interfaces->m_move_helper->process_impacts( );
    c_player::get_local( )->post_think( );
}

void c_eng_pred::end( ) const {
    if ( !c_player::get_local( ) || !c_player::get_local( )->is_alive( ) || !g_ctx->local( ).m_cmd )
        return;

    m_backup_data.reset( );

    g_interfaces->m_game_movement->finish_track_prediction_errors( c_player::get_local( ) );

    c_player::get_local( )->current_cmd( ) = nullptr;
    c_player::get_local( )->set_pred_random_seed( nullptr );
    c_player::get_local( )->set_pred_player( nullptr );

    g_interfaces->m_game_movement->reset( );

    g_interfaces->m_move_helper->set_host( nullptr );
}

void c_eng_pred::update( ) {
    if ( g_interfaces->m_client_state->m_delta_tick < 0 )
        return;

    if ( g_interfaces->m_client_state->m_signon_state != 6 )
        return;

    return g_interfaces->m_prediction->update( g_interfaces->m_client_state->m_delta_tick,
                                               true,
                                               g_interfaces->m_client_state->m_last_command_ack,
                                               g_interfaces->m_client_state->m_choked_cmds
                                                   + g_interfaces->m_client_state->m_last_outgoing_command );
}