#include "config_manager.h"
#include "../xor.h"

void cfg_manager::save(const std::string cfg_name)
{
	// write data
	nlohmann::json j = {
		{ _("legit::m_enabled"),	g_cfg.m_legit.m_enabled },
		{ _("legit::m_auto_fire"),	g_cfg.m_legit.m_auto_fire },
		{ _("legit::m_auto_scope"), g_cfg.m_legit.m_auto_scope },
		{ _("legit::m_silent"),		g_cfg.m_legit.m_silent },
		{ _("legit::m_fov"),		g_cfg.m_legit.m_fov },
		{ _("legit::m_smooth"),		g_cfg.m_legit.m_smooth },
		{ _("legit::m_rcs_x"),		g_cfg.m_legit.m_rcs_x },
		{ _("legit::m_rcs_y"),		g_cfg.m_legit.m_rcs_y },

		{ _("misc::m_bunny_hop"),		g_cfg.m_misc.m_bunny_hop },
		{ _("misc::m_no_scope"),		g_cfg.m_misc.m_no_scope },
		{ _("misc::m_infinity_duck"),	g_cfg.m_misc.m_infinity_duck },
		{ _("misc::m_remove_molotov"),	g_cfg.m_misc.m_remove_molotov },
		{ _("misc::m_fov"),				g_cfg.m_misc.m_fov },
		{ _("misc::m_third_person_distance"),			g_cfg.m_misc.m_third_person_distance },
		{ _("misc::m_third_person_bind_key_selected"),	g_cfg.m_misc.m_third_person_bind.m_key_selected },
		{ _("misc::m_third_person_bind_mode_selected"), g_cfg.m_misc.m_third_person_bind.m_mode_selected },
	
		{ _("anti_aim::m_enabled"),				g_cfg.m_anti_aim.m_enabled },
		{ _("anti_aim::m_desync"),				g_cfg.m_anti_aim.m_desync },
		{ _("anti_aim::m_fake_lags"),			g_cfg.m_anti_aim.m_fake_lags },
		{ _("anti_aim::m_desync_right_range"),	g_cfg.m_anti_aim.m_desync_right_range },
		{ _("anti_aim::m_desync_left_range"),	g_cfg.m_anti_aim.m_desync_left_range },
		{ _("anti_aim::m_fake_lags_factor"),	g_cfg.m_anti_aim.m_fake_lags_factor },
		{ _("anti_aim::m_pitch_mode"),			g_cfg.m_anti_aim.m_pitch_mode },
		{ _("anti_aim::m_desync_mode"),			g_cfg.m_anti_aim.m_desync_mode },
		{ _("anti_aim::m_inverter_key_selected"),	g_cfg.m_anti_aim.m_inverter.m_key_selected },
		{ _("anti_aim::m_inverter_mode_selected"),	g_cfg.m_anti_aim.m_inverter.m_mode_selected },
	};

	for (int i{}; i <= 3; i++)
	{
		j[_("esp::")][std::to_string(i)][_("m_name")]				= g_cfg.m_esp.m_player[i].m_name;
		j[_("esp::")][std::to_string(i)][_("m_box")]				= g_cfg.m_esp.m_player[i].m_box;
		j[_("esp::")][std::to_string(i)][_("m_health_bar")]			= g_cfg.m_esp.m_player[i].m_health_bar;
		j[_("esp::")][std::to_string(i)][_("m_health_text")]		= g_cfg.m_esp.m_player[i].m_health_text;
		j[_("esp::")][std::to_string(i)][_("m_chams")]				= g_cfg.m_esp.m_player[i].m_chams;
		j[_("esp::")][std::to_string(i)][_("m_invisible_chams")]	= g_cfg.m_esp.m_player[i].m_invisible_chams;
	}

	// write data to file
	write_file(j, cfg_name);
}

void cfg_manager::write_file(nlohmann::json cfg, const std::string cfg_name)
{
	// create an object to write to a file
	std::ofstream file(_("C:\\") + cfg_name + _(".json"), std::ios::out | std::ios::trunc);
	
	if (!file.good())
		return;

	file << std::setw(4) << cfg << std::endl;
	file.close();
}

void cfg_manager::load(const std::string cfg_name)
{
	// open file for reading
	std::ifstream file(_("C:\\") + cfg_name + _(".json"), std::ios::in);
	
	if (!file.good())
		return;

	// read file
	nlohmann::json j;
	file >> j;

	// load
	g_cfg.m_legit.m_enabled		= j[_("legit::m_enabled")];
	g_cfg.m_legit.m_auto_fire	= j[_("legit::m_auto_fire")];
	g_cfg.m_legit.m_auto_scope	= j[_("legit::m_silent")];
	g_cfg.m_legit.m_fov			= j[_("legit::m_fov")];
	g_cfg.m_legit.m_smooth		= j[_("legit::m_smooth")];
	g_cfg.m_legit.m_rcs_x		= j[_("legit::m_rcs_x")];
	g_cfg.m_legit.m_rcs_y		= j[_("legit::m_rcs_y")];

	g_cfg.m_misc.m_bunny_hop		= j[_("misc::m_bunny_hop")];
	g_cfg.m_misc.m_no_scope			= j[_("misc::m_no_scope")];
	g_cfg.m_misc.m_infinity_duck	= j[_("misc::m_infinity_duck")];
	g_cfg.m_misc.m_remove_molotov	= j[_("misc::m_remove_molotov")];
	g_cfg.m_misc.m_fov				= j[_("misc::m_fov")];
	g_cfg.m_misc.m_third_person_distance				= j[_("misc::m_third_person_distance")];
	g_cfg.m_misc.m_third_person_bind.m_key_selected		= j[_("misc::m_third_person_bind_key_selected")];
	g_cfg.m_misc.m_third_person_bind.m_mode_selected	= j[_("misc::m_third_person_bind_mode_selected")];

	g_cfg.m_anti_aim.m_enabled					= j[_("anti_aim::m_enabled")];
	g_cfg.m_anti_aim.m_desync					= j[_("anti_aim::m_desync")];
	g_cfg.m_anti_aim.m_fake_lags				= j[_("anti_aim::m_fake_lags")];
	g_cfg.m_anti_aim.m_desync_right_range		= j[_("anti_aim::m_desync_right_range")];
	g_cfg.m_anti_aim.m_desync_left_range		= j[_("anti_aim::m_desync_left_range")];
	g_cfg.m_anti_aim.m_fake_lags_factor			= j[_("anti_aim::m_fake_lags_factor")];
	g_cfg.m_anti_aim.m_pitch_mode				= j[_("anti_aim::m_pitch_mode")];
	g_cfg.m_anti_aim.m_desync_mode				= j[_("anti_aim::m_desync_mode")];
	g_cfg.m_anti_aim.m_inverter.m_key_selected	= j[_("anti_aim::m_inverter_key_selected")];
	g_cfg.m_anti_aim.m_inverter.m_mode_selected = j[_("anti_aim::m_inverter_mode_selected")];

	for (int i{}; i <= 3; i++)
	{
		g_cfg.m_esp.m_player[i].m_name				= j[_("esp::")][std::to_string(i)][_("m_name")];
		g_cfg.m_esp.m_player[i].m_box				= j[_("esp::")][std::to_string(i)][_("m_box")];
		g_cfg.m_esp.m_player[i].m_health_bar		= j[_("esp::")][std::to_string(i)][_("m_health_bar")];
		g_cfg.m_esp.m_player[i].m_health_text		= j[_("esp::")][std::to_string(i)][_("m_health_text")];
		g_cfg.m_esp.m_player[i].m_chams				= j[_("esp::")][std::to_string(i)][_("m_chams")];
		g_cfg.m_esp.m_player[i].m_invisible_chams	= j[_("esp::")][std::to_string(i)][_("m_invisible_chams")];
	}

	file.close();
}