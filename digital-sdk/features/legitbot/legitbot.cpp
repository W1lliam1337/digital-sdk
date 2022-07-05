#include "legitbot.h"
#include "../autowall/autowall.h"

void c_legit_bot::init()
{
	if (!g_cfg.m_legit.m_enabled)
		return;

	if (!g_sdk.m_local() || !g_sdk.m_local()->is_alive())
		return;

	const auto active_weapon = g_sdk.m_local()->get_active_weapon();
	if (!active_weapon)
		return;

	aimbot(active_weapon);
}

void c_legit_bot::aimbot(c_weapon* active_weapon)
{
	auto best_fov = g_cfg.m_legit.m_fov;
	auto best_angle = qangle_t{ 0, 0, 0 };

	const auto shoot_pos = g_sdk.m_local()->get_shoot_pos();

	for (int i{ 1 }; i < g_interfaces.m_globals->m_max_clients; i++)
	{
		const auto entity = static_cast<c_base_player*>(g_interfaces.m_entity_list->get_client_entity(i));
		if (!entity || !entity->is_alive() || entity->is_dormant()
			|| entity->get_team() == g_sdk.m_local()->get_team())
			continue;

		for (const auto hitbox :
			{
				 hitbox_head,
				 hitbox_neck,
				 hitbox_pelvis,
				 hitbox_stomach,
				 hitbox_lower_chest,
				 hitbox_chest,
				 hitbox_upper_chest,
				 hitbox_right_thigh,
				 hitbox_left_thigh,
				 hitbox_right_calf,
				 hitbox_left_calf,
			})
		{
			const auto hitbox_pos = entity->get_hitbox_pos(hitbox);

			c_game_trace trace;
			c_trace_filter filter;
			filter.m_skip = g_sdk.m_local();

			ray_t ray;
			ray.init(g_sdk.m_local()->get_shoot_pos(), hitbox_pos);

			g_interfaces.m_trace->trace_ray(ray, mask_shot, &filter, &trace);

			if (!trace.is_visible())
				continue;

			if (g_cfg.m_legit.m_auto_fire)
				g_sdk.m_packet_data.m_cmd->m_buttons |= in_attack;

			const auto enemy_angle_x = (hitbox_pos - shoot_pos).to_angle().x - g_sdk.m_packet_data.m_cmd->m_view_angles.x + g_sdk.m_local()->get_aim_punch_angle().x;
			const auto enemy_angle_y = (hitbox_pos - shoot_pos).to_angle().y - g_sdk.m_packet_data.m_cmd->m_view_angles.y + g_sdk.m_local()->get_aim_punch_angle().y;

			const auto fov = std::hypot(enemy_angle_x, enemy_angle_y);

			if (fov < best_fov)
			{
				best_fov = fov;
				best_angle = { enemy_angle_x, enemy_angle_y, 0.0f };
			}
		}
	}

	const auto smooth = g_cfg.m_legit.m_silent ? 1 : g_cfg.m_legit.m_smooth;
	g_sdk.m_packet_data.m_cmd->m_view_angles += best_angle / static_cast<float>(smooth);

	if (!g_cfg.m_legit.m_silent)
		g_interfaces.m_engine->set_view_angles(g_sdk.m_packet_data.m_cmd->m_view_angles);
}
