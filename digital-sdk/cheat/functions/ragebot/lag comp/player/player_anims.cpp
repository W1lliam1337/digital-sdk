#include "player_anims.h"

void c_player_lagcomp::on_frame()
{
	for (int i{}; i < g_sdk.m_interfaces.m_globals->m_max_clients; i++)
	{
		const auto entity = static_cast<c_base_player*>(g_sdk.m_interfaces.m_entity_list->get_client_entity(i));
		if (!entity || !entity->is_alive() || entity->get_team() == g_sdk.m_local()->get_team())
			continue;

        if (entity->is_dormant())
            continue;
      
        if (m_current_records[i].empty() || entity->get_simulation_time() != entity->get_old_simulation_time())
        {
            if (entity->get_old_simulation_time() > entity->get_simulation_time()) 
            {
                m_current_records[i].clear();
                continue;
            }

            if (!m_current_records[i].empty() && entity->get_anim_layers()[11].m_cycle == m_current_records[i].front().m_animation_layers.at(11).m_cycle)
            {
                m_current_records[i].clear();
                continue;
            }

            if (!m_current_records[i].empty() && (entity->get_origin() - m_current_records[i].front().m_origin).length_sqr() > 4096.0f)
            {
                m_current_records[i].clear();
                continue;
            }

            c_lag_record lag_record;
            fill_record(entity, lag_record);
            m_current_records[i].emplace_front(lag_record);
          
            std::array < animlayer_t, animation_layer_count > animation_layers{};
            c_animstate animstate;

            memcpy(animation_layers.data(), entity->get_anim_layers(), sizeof(animlayer_t) * animation_layer_count);
            memcpy(&animstate, entity->get_anim_state(), sizeof(c_animstate));
            {
                update_player(entity);
            }
            memcpy(entity->get_anim_state(), &animstate, sizeof(c_animstate));
            memcpy(entity->get_anim_layers(), animation_layers.data(), sizeof(animlayer_t) * animation_layer_count);

            build_matrix(entity, lag_record.m_matrices.at(main_matrix).data(), bone_used_by_anything);
            memcpy(entity->get_bone_cache().Base(), lag_record.m_matrices.at(main_matrix).data(), sizeof(matrix_t) * entity->get_bone_cache().Count());
        }

        while (m_current_records[i].size() > 32)
            m_current_records[i].pop_front();
	}
}

void c_player_lagcomp::fill_record(c_base_player* player, c_lag_record& lag_record)
{
    memcpy(lag_record.m_animation_layers.data(), player->get_anim_layers(), sizeof(animlayer_t) * animation_layer_count);
    memcpy(lag_record.m_matrices.at(main_matrix).data(), player->get_bone_cache().Base(), player->get_bone_cache().Count() * sizeof(matrix_t));

    lag_record.m_duck_amount = player->get_duck_amount();
    lag_record.m_simulation_time = player->get_simulation_time();
    lag_record.m_lower_body_yaw = player->get_lby();
   
    lag_record.m_flags = player->get_flags();
    lag_record.m_choked_ticks = 1;

    lag_record.m_velocity = player->get_velocity();
    lag_record.m_origin = player->get_origin();
    lag_record.m_abs_origin = player->get_abs_origin();
    lag_record.m_maxs = player->get_collideable()->obb_maxs();
    lag_record.m_mins = player->get_collideable()->obb_mins();
    lag_record.m_eye_angles = player->get_eye_angles();
}

void c_player_lagcomp::build_matrix(c_base_player* player, matrix_t* matrix, const int mask)
{
    const auto cur_time = g_sdk.m_interfaces.m_globals->m_current_time;
    const auto real_time = g_sdk.m_interfaces.m_globals->m_realtime;
    const auto frame_time = g_sdk.m_interfaces.m_globals->m_frame_time;
    const auto abs_frame_time = g_sdk.m_interfaces.m_globals->m_abs_frame_time;
    const auto framecount = g_sdk.m_interfaces.m_globals->m_frame_count;
    const auto tickcount = g_sdk.m_interfaces.m_globals->m_tick_count;
    const auto interpolation = g_sdk.m_interfaces.m_globals->m_interpolation_amount;

    g_sdk.m_interfaces.m_globals->m_current_time = player->get_simulation_time();
    g_sdk.m_interfaces.m_globals->m_realtime = player->get_simulation_time();
    g_sdk.m_interfaces.m_globals->m_frame_time = g_sdk.m_interfaces.m_globals->m_interval_per_tick;
    g_sdk.m_interfaces.m_globals->m_abs_frame_time = g_sdk.m_interfaces.m_globals->m_interval_per_tick;
    g_sdk.m_interfaces.m_globals->m_frame_count = INT_MAX;
    g_sdk.m_interfaces.m_globals->m_tick_count = TIME_TO_TICKS(player->get_simulation_time());
    g_sdk.m_interfaces.m_globals->m_interpolation_amount = 0.0f;
  
    const auto& abs_origin = player->get_abs_origin();

    player->invalidate_bone_cache();
    player->get_bone_accessor().m_ReadableBones = 0;
    player->get_bone_accessor().m_WritableBones = 0;
    player->get_effects().add(8);

    if (player != g_sdk.m_local())
        player->set_abs_origin(player->get_origin());

    g_sdk.m_player_data.m_setup_bones = true;
    player->setup_bones(matrix, MAXSTUDIOBONES, mask, player->get_simulation_time());
    g_sdk.m_player_data.m_setup_bones = false;
   
    if (player != g_sdk.m_local())
        player->set_abs_origin(abs_origin);

    player->get_effects().remove(8);

    g_sdk.m_interfaces.m_globals->m_current_time = cur_time;
    g_sdk.m_interfaces.m_globals->m_realtime = real_time;
    g_sdk.m_interfaces.m_globals->m_frame_time = frame_time;
    g_sdk.m_interfaces.m_globals->m_abs_frame_time = abs_frame_time;
    g_sdk.m_interfaces.m_globals->m_frame_count = framecount;
    g_sdk.m_interfaces.m_globals->m_tick_count = tickcount;
    g_sdk.m_interfaces.m_globals->m_interpolation_amount = interpolation;
}

void c_player_lagcomp::update_player(c_base_player* player)
{
    if (m_current_records[player->get_index()].empty())
        return;

    if (m_current_records[player->get_index()].size() >= 2)
        m_previous_record = &m_current_records[player->get_index()].at(1);

    const auto current_record = &m_current_records[player->get_index()].front();
    const auto previous_record = m_previous_record;
}