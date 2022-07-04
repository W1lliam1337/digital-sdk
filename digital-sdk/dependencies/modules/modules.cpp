#include "modules.h"

void c_modules::init() const
{
	g_sdk.m_modules.m_client_dll = GetModuleHandleA(_("client.dll"));
	g_sdk.m_modules.m_engine_dll = GetModuleHandleA(_("engine.dll"));
	g_sdk.m_modules.m_tier_dll = GetModuleHandleA(_("tier0.dll"));
	g_sdk.m_modules.m_localize_dll = GetModuleHandleA(_("localize.dll"));
	g_sdk.m_modules.m_file_system_dll = GetModuleHandleA(_("filesystem_stdio.dll"));
	g_sdk.m_modules.m_shader_dll = GetModuleHandleA(_("shaderapidx9.dll"));
	g_sdk.m_modules.m_vgui_dll = GetModuleHandleA(_("vguimatsurface.dll"));
	g_sdk.m_modules.m_vgui2_dll = GetModuleHandleA(_("vgui2.dll"));
	g_sdk.m_modules.m_physics_dll = GetModuleHandleA(_("vphysics.dll"));
	g_sdk.m_modules.m_vstd_dll = GetModuleHandleA(_("vstdlib.dll"));
	g_sdk.m_modules.m_mat_sys_dll = GetModuleHandleA(_("materialsystem.dll"));
	g_sdk.m_modules.m_data_cache_dll = GetModuleHandleA(_("datacache.dll"));
	g_sdk.m_modules.m_studio_render_dll = GetModuleHandleA(_("studiorender.dll"));
	g_sdk.m_modules.m_server_dll = GetModuleHandleA(_("server.dll"));
}
