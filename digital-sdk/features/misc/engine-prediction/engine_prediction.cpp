#include "engine_prediction.h"

void c_engine_prediction::setup()
{
	static auto prediction_player = c_utils::find_sig(g_modules.m_client_dll, _("8B 47 40 A3")) + 4;
	static auto prediction_seed = c_utils::find_sig(g_modules.m_client_dll, _("0F 5B C0 89 35")) + 5;

	/* setup prediction data */
	m_prediction_player = *reinterpret_cast<std::uintptr_t*>(prediction_player);
	m_prediction_seed = *reinterpret_cast<std::uintptr_t*>(prediction_seed);

	/* setup globals backup data */
	m_backup_data.m_tick_count = g_interfaces.m_globals->m_tick_count;
	m_backup_data.m_frame_count = g_interfaces.m_globals->m_frame_count;
	m_backup_data.m_current_time = g_interfaces.m_globals->m_current_time;
	m_backup_data.m_frame_time = g_interfaces.m_globals->m_frame_time;

	/* setup prediction backup data */
	m_backup_data.m_in_prediction = g_interfaces.m_prediction->m_in_prediction;
	m_backup_data.m_is_first_time_predicted = g_interfaces.m_prediction->m_first_time_predicted;
}

void c_engine_prediction::begin()
{
	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L899 */
	*reinterpret_cast<c_user_cmd**>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3348) = g_sdk.m_packet_data.m_cmd;
	*reinterpret_cast<c_user_cmd**>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3298) = g_sdk.m_packet_data.m_cmd; //set(player + 0x3298, cmd);

	g_sdk.m_packet_data.m_cmd->m_buttons |= *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3344);
	g_sdk.m_packet_data.m_cmd->m_buttons &= ~*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3340);

	/* setup prediction vars */
	*reinterpret_cast<int*>(m_prediction_player) = reinterpret_cast<std::uintptr_t>(g_sdk.m_local());
	*reinterpret_cast<int*>(m_prediction_seed) = g_sdk.m_packet_data.m_cmd ? g_sdk.m_packet_data.m_cmd->m_random_seed : -1;

	/* fix times */
	g_interfaces.m_globals->m_current_time = TICKS_TO_TIME(g_sdk.m_local()->get_tick_base());
	g_interfaces.m_globals->m_frame_time = g_interfaces.m_prediction->m_engine_paused
		? 0.0f
		: g_interfaces.m_globals->m_interval_per_tick;
	g_interfaces.m_globals->m_tick_count = g_sdk.m_local()->get_tick_base();

	/* setup vars */
	g_interfaces.m_prediction->m_in_prediction = true;
	g_interfaces.m_prediction->m_first_time_predicted = false;

	g_interfaces.m_game_movement->start_track_prediction_errors(g_sdk.m_local());
	g_interfaces.m_move_helper->set_host(g_sdk.m_local());

	/* @fix-me: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L1067 */
	//if (g_sdk.m_packet_data.m_cmd->m_weapon_select != 0)
	//	if (const auto& weapon = g_sdk.m_local()->get_active_weapon())
	//		g_sdk.m_local()->select_item(weapon->get_name().c_str(), g_sdk.m_packet_data.m_cmd->m_weapon_select);

	const auto vehicle = g_sdk.m_local()->get_vehicle().get();

	/* impulse fix */
	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L1078 */
	if (g_sdk.m_packet_data.m_cmd->m_impulse && (!vehicle || g_sdk.m_local()->using_standard_weapons_in_vehicle()))
		g_sdk.m_local()->get_impulse() = g_sdk.m_packet_data.m_cmd->m_impulse;

	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/29e4c1fda9698d5cebcdaf1a0de4b829fa149bf8/game/shared/baseplayer_shared.cpp#L961 */
	const int buttons_changed = g_sdk.m_packet_data.m_cmd->m_buttons ^ *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3208);

	*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x31FC) = *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3208);
	*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3208) = g_sdk.m_packet_data.m_cmd->m_buttons;
	*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3200) = g_sdk.m_packet_data.m_cmd->m_buttons & buttons_changed;
	*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0x3204) = buttons_changed & ~g_sdk.m_packet_data.m_cmd->m_buttons;

	g_interfaces.m_prediction->check_moving_ground(g_sdk.m_local(), g_interfaces.m_globals->m_frame_time);

	if (g_sdk.m_local()->physics_run_think(0))
		g_sdk.m_local()->get_pre_think();

	/* @note: you can use (int m_nNextThinkTick) */
	auto next_think_tick = *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(g_sdk.m_local()) + 0xFC);

	if (next_think_tick > 0 && next_think_tick <= g_sdk.m_local()->get_tick_base())
	{
		next_think_tick = -1;
		g_sdk.m_local()->get_think();
	}

	g_interfaces.m_prediction->setup_move(g_sdk.m_local(), g_sdk.m_packet_data.m_cmd, g_interfaces.m_move_helper, &m_move_data);
	g_interfaces.m_prediction->set_local_view_angles(g_sdk.m_packet_data.m_cmd->m_view_angles);
	g_interfaces.m_game_movement->process_movement(g_sdk.m_local(), &m_move_data);
	g_interfaces.m_prediction->finish_move(g_sdk.m_local(), g_sdk.m_packet_data.m_cmd, &m_move_data);
	g_interfaces.m_move_helper->process_impacts();
	g_interfaces.m_move_helper->set_host(nullptr);
	g_interfaces.m_game_movement->finish_track_prediction_errors(g_sdk.m_local());
	g_interfaces.m_game_movement->reset();

	/* @note: here your inaccuracy calculation (https://yougame.biz/threads/252551/ 78 line) */
	if (g_sdk.m_local()->get_active_weapon())
		g_sdk.m_local()->get_active_weapon()->update_accuracy_penalty();

	g_sdk.m_local_data.m_shoot_pos = g_sdk.m_local()->get_shoot_pos();
}

void c_engine_prediction::end() const
{
	if (!m_prediction_player || !m_prediction_seed)
		return;

	/* restore globals to backup data */
	g_interfaces.m_globals->m_current_time = m_backup_data.m_current_time;
	g_interfaces.m_globals->m_frame_time = m_backup_data.m_frame_time;
	g_interfaces.m_globals->m_tick_count = m_backup_data.m_tick_count;

	/* restore prediction data */
	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/prediction.cpp#L919 */
	*reinterpret_cast<c_user_cmd**>(reinterpret_cast<uintptr_t>(g_sdk.m_local()) + 0x3348) = nullptr;
	*reinterpret_cast<int*>(m_prediction_player) = -1;
	*reinterpret_cast<int*>(m_prediction_seed) = -1;
}