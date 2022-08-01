#include "anti_aim.h"

void anti_aim::init()
{
	if (cant_work())
		return;

	pitch();
	yaw();
}

bool anti_aim::cant_work()
{
	if (!g_cfg.m_anti_aim.m_enabled)
		return true;

	if (!ctx::packet_data::m_cmd)
		return true;

	if (!ctx::local()->is_alive())
		return true;

	if (ctx::local()->get_flags() & fl_frozen)
		return true;

	if (ctx::local()->get_move_type() == move_type_noclip
		|| ctx::local()->get_move_type() == move_type_ladder)
		return true;

	if (!ctx::local()->get_active_weapon())
		return true;

	if (ctx::packet_data::m_cmd->m_buttons & in_attack && ctx::local()->get_active_weapon()->get_idx() != weapon_revolver)
		return true;

	if ((ctx::packet_data::m_cmd->m_buttons & in_attack || ctx::packet_data::m_cmd->m_buttons & in_attack2) && ctx::local()->get_active_weapon()->is_knife())
		return true;

	if (ctx::packet_data::m_cmd->m_buttons & in_use)
		return true;

	if (ctx::local()->is_defusing())
		return true;

	if (ctx::local()->get_active_weapon()->get_throw_time())
		return true;

	return false;
}

void anti_aim::pitch()
{
	float modifier_value{};

	switch (g_cfg.m_anti_aim.m_pitch_mode)
	{
	case e_pitch_mode::down:
	{ modifier_value = 89.0f; } break;
	case e_pitch_mode::up:
	{ modifier_value = -89.0f; } break;
	default: break;
	}

	ctx::packet_data::m_cmd->m_view_angles.x = modifier_value;
}

void anti_aim::yaw()
{
	if (ctx::packet_data::m_send_packet)
		m_desync_side = utils::is_bind_active(g_cfg.m_anti_aim.m_inverter) ? e_desync_side::left : e_desync_side::right;

	ctx::packet_data::m_cmd->m_view_angles.y = math::normalize_yaw(ctx::packet_data::m_cmd->m_view_angles.y + 180.0f);

	if (!ctx::packet_data::m_send_packet)
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

		ctx::packet_data::m_cmd->m_view_angles.y += desync_amount;
	}
}