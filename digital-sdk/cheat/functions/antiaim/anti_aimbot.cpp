#include "anti_aimbot.h"

void c_anti_aimbot::init()
{
	if (cant_work())
		return;

	pitch();
	yaw();
}

bool c_anti_aimbot::cant_work()
{
	if (!g_cfg.m_anti_aim.m_enable)
		return true;

	if (!g_sdk.m_packet_data.m_cmd)
		return true;

	if (!g_sdk.m_local()->is_alive())
		return true;

	if (g_sdk.m_local()->get_flags() & fl_frozen)
		return true;

	if (g_sdk.m_local()->get_move_type() == move_type_noclip 
		|| g_sdk.m_local()->get_move_type() == move_type_ladder)
		return true;

	if (!g_sdk.m_local()->get_active_weapon())
		return true;

	if (g_sdk.m_packet_data.m_cmd->m_buttons & in_attack && g_sdk.m_local()->get_active_weapon()->get_idx() != weapon_revolver)
		return true;

	if ((g_sdk.m_packet_data.m_cmd->m_buttons & in_attack || g_sdk.m_packet_data.m_cmd->m_buttons & in_attack2) && g_sdk.m_local()->get_active_weapon()->is_knife())
		return true;

	if (g_sdk.m_packet_data.m_cmd->m_buttons & in_use)
		return true;

	if (g_sdk.m_local()->is_defusing())
		return true;

	if (g_sdk.m_local()->get_active_weapon()->get_throw_time())
		return true;

	return false;
}

void c_anti_aimbot::pitch()
{
	float modifier_value{};

	switch (g_cfg.m_anti_aim.m_pitch_mode)
	{
	case e_pitch_mode::down:
	{ modifier_value = -89.0f; } break;
	case e_pitch_mode::up:
	{ modifier_value = 89.0f; } break;
	default: break;
	}

	g_sdk.m_packet_data.m_cmd->m_view_angles.x = modifier_value;
}

void c_anti_aimbot::yaw()
{
	if (g_sdk.m_packet_data.m_send_packet)
	{
		m_desync_side = /*c_utils::get()->is_bind_active(g_cfg.m_anti_aim.m_inverter) ? e_desync_side::left : */e_desync_side::right;
	}

	g_sdk.m_packet_data.m_cmd->m_view_angles.y = c_math::normalize_yaw(g_sdk.m_packet_data.m_cmd->m_view_angles.y + 180.0f);


	if (!g_sdk.m_packet_data.m_send_packet)
	{
		float desync_amount = 0.f;
		switch (g_cfg.m_anti_aim.m_desync_mode)
		{
		case e_desync_mode::custom:
			{
				desync_amount = static_cast<float>((m_desync_side > 0
					                                    ? g_cfg.m_anti_aim.m_desync_right_range
					                                    : g_cfg.m_anti_aim.m_desync_left_range));
			}
		default: break;
		}

		g_sdk.m_packet_data.m_cmd->m_view_angles.y += desync_amount;
	}
}