#include "engine_prediction.h"

void engine_prediction::setup( ) {
	static const auto prediction_player = utils::sig( modules::m_client_dll, _( "8B 47 40 A3" ) ) + 4;
	static const auto prediction_seed = utils::sig( modules::m_client_dll, _( "0F 5B C0 89 35" ) ) + 5;

	// setup prediction data
	m_prediction_player = *reinterpret_cast<std::uintptr_t*>(prediction_player);
	m_prediction_seed = *reinterpret_cast<std::uintptr_t*>(prediction_seed);

	// setup globals backup data
	m_backup_data.m_tick_count = interfaces::m_globals->m_tick_count;
	m_backup_data.m_frame_count = interfaces::m_globals->m_frame_count;
	m_backup_data.m_current_time = interfaces::m_globals->m_current_time;
	m_backup_data.m_frame_time = interfaces::m_globals->m_frame_time;

	// setup prediction backup data
	m_backup_data.m_in_prediction = interfaces::m_prediction->m_in_prediction;
	m_backup_data.m_is_first_time_predicted = interfaces::m_prediction->m_first_time_predicted;
}

// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L1948
void engine_prediction::update( ) {
	if ( interfaces::m_client_state->m_delta_tick < 0 )
		return;

	if ( interfaces::m_client_state->m_signon_state != 6 )
		return;

	return interfaces::m_prediction->update
	(
		interfaces::m_client_state->m_delta_tick,
		true, // interfaces::m_client_state->m_delta_tick > 0
		interfaces::m_client_state->m_last_command_ack,
		interfaces::m_client_state->m_choked_commands + interfaces::m_client_state->m_last_outgoing_command
	);
}

void engine_prediction::begin( ) {
	// setup command 
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L899
	ctx::local( )->get_current_cmd( ) = ctx::packet_data::m_cmd;
	ctx::local( )->get_last_cmd( ) = ctx::packet_data::m_cmd;

	// setup prediction vars
	*reinterpret_cast<int*>(m_prediction_player) = reinterpret_cast<std::uintptr_t>(ctx::local( ));
	*reinterpret_cast<int*>(m_prediction_seed) = ctx::packet_data::m_cmd ? ctx::packet_data::m_cmd->m_random_seed : -1;

	// setup globals
	interfaces::m_globals->m_current_time = ctx::ticks_to_time( ctx::local( )->get_tick_base( ) );
	interfaces::m_globals->m_frame_time = interfaces::m_prediction->m_engine_paused
		? 0.0f
		: interfaces::m_globals->m_interval_per_tick;
	interfaces::m_globals->m_tick_count = ctx::local( )->get_tick_base( );

	// add and subtract buttons we're forcing on the player
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L1058
	ctx::packet_data::m_cmd->m_buttons |= ctx::local( )->get_button_forced( );
	ctx::packet_data::m_cmd->m_buttons &= ~ctx::local( )->get_button_disabled( );

	// setup vars
	interfaces::m_prediction->m_in_prediction = true;
	interfaces::m_prediction->m_first_time_predicted = false;

	interfaces::m_game_movement->start_track_prediction_errors( ctx::local( ) );
	interfaces::m_move_helper->set_host( ctx::local( ) );

	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L1067
	if ( ctx::packet_data::m_cmd->m_weapon_select != 0 )
		if ( const auto& weapon = ctx::local( )->get_active_weapon( ) )
			ctx::local( )->select_item( weapon->get_weapon_data( )->m_weapon_name, ctx::packet_data::m_cmd->m_weapon_select );

	// impulse fix
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L1078
	if ( const auto vehicle = ctx::local( )->get_vehicle( ).get( );
		 ctx::packet_data::m_cmd->m_impulse && (!vehicle || ctx::local( )->using_standard_weapons_in_vehicle( )) ) {
		ctx::local( )->get_impulse( ) = ctx::packet_data::m_cmd->m_impulse;
	}

	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/29e4c1fda9698d5cebcdaf1a0de4b829fa149bf8/game/shared/baseplayer_shared.cpp#L961
	ctx::local( )->get_button_last( ) = ctx::local( )->get_buttons( );
	ctx::local( )->get_buttons( ) = ctx::packet_data::m_cmd->m_buttons;

	const int buttons_changed = ctx::packet_data::m_cmd->m_buttons ^ ctx::local( )->get_buttons( );
	ctx::local( )->get_button_pressed( ) = ctx::packet_data::m_cmd->m_buttons & buttons_changed;
	ctx::local( )->get_button_released( ) = buttons_changed & ~ctx::packet_data::m_cmd->m_buttons;

	interfaces::m_prediction->check_moving_ground( ctx::local( ), interfaces::m_globals->m_frame_time );

	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L935
	if ( ctx::local( )->physics_run_think( ) )
		ctx::local( )->pre_think( );

	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L962
	if ( ctx::local( )->get_next_think_tick( ) > 0 && ctx::local( )->get_next_think_tick( ) <= ctx::local( )->get_tick_base( ) ) {
		ctx::local( )->get_next_think_tick( ) = -1;
		ctx::local( )->think( );
	}

	interfaces::m_prediction->setup_move( ctx::local( ), ctx::packet_data::m_cmd, interfaces::m_move_helper, &m_move_data );
	interfaces::m_prediction->set_local_view_angles( ctx::packet_data::m_cmd->m_view_angles );
	interfaces::m_game_movement->process_movement( ctx::local( ), &m_move_data );
	interfaces::m_prediction->finish_move( ctx::local( ), ctx::packet_data::m_cmd, &m_move_data );
	interfaces::m_move_helper->process_impacts( );

	ctx::local( )->post_think( );

	// @note: here your inaccuracy calculation
	if ( ctx::local( )->get_active_weapon( ) )
		ctx::local( )->get_active_weapon( )->update_accuracy_penalty( );

	ctx::local_data::m_shoot_pos = ctx::local( )->get_shoot_pos( );
}

void engine_prediction::end( ) {
	if ( !m_prediction_player || !m_prediction_seed )
		return;

	// restore globals to backup data
	interfaces::m_globals->m_current_time = m_backup_data.m_current_time;
	interfaces::m_globals->m_frame_time = m_backup_data.m_frame_time;
	interfaces::m_globals->m_tick_count = m_backup_data.m_tick_count;

	// restore prediction data
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L919
	interfaces::m_game_movement->finish_track_prediction_errors( ctx::local( ) );

	ctx::local( )->get_current_cmd( ) = nullptr;
	*reinterpret_cast<int*>(m_prediction_player) = -1;
	*reinterpret_cast<int*>(m_prediction_seed) = -1;

	interfaces::m_game_movement->reset( );

	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/c_baseplayer.cpp#L2936
	interfaces::m_move_helper->set_host( nullptr );
}