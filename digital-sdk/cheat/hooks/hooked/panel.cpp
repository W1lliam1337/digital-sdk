#include "../hooks.h"

void __fastcall hooks::hk_paint_traverse(void* ecx, void* edx, const vgui::vpanel panel, const bool force_repaint, const bool allow_force)
{
	static uint32_t hud_zoom_panel = 0;

	if (!hud_zoom_panel)
		if (!strcmp(_("HudZoom"), interfaces::m_panel->get_name(panel)))
			hud_zoom_panel = panel;

	if (hud_zoom_panel == panel && g_cfg.m_misc.m_no_scope)
		return;

	hooks::og::m_paint_traverse(ecx, edx, panel, force_repaint, allow_force);

	static vgui::vpanel panel_id = 0;

	if (!panel_id)
	{
		if (!strcmp(interfaces::m_panel->get_name(panel), "MatSystemTopPanel"))
			panel_id = panel;
	}

	if (panel_id != panel)
		return;

	render::begin();
	{
		logs::instance();
		player_esp::draw();
	}
	render::end();
}

