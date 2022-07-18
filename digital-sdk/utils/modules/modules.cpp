#include "modules.h"

void c_modules::init() noexcept
{
	/* @ref: https://docs.microsoft.com/en-us/windows/win32/dlls/run-time-dynamic-linking */
	m_client_dll		= LoadLibraryA(_("client.dll"));
	m_engine_dll		= LoadLibraryA(_("engine.dll"));
	m_tier_dll			= LoadLibraryA(_("tier0.dll"));
	m_localize_dll		= LoadLibraryA(_("localize.dll"));
	m_file_system_dll	= LoadLibraryA(_("filesystem_stdio.dll"));
	m_shader_dll		= LoadLibraryA(_("shaderapidx9.dll"));
	m_vgui_dll			= LoadLibraryA(_("vguimatsurface.dll"));
	m_vgui2_dll			= LoadLibraryA(_("vgui2.dll"));
	m_physics_dll		= LoadLibraryA(_("vphysics.dll"));
	m_vstd_dll			= LoadLibraryA(_("vstdlib.dll"));
	m_mat_sys_dll		= LoadLibraryA(_("materialsystem.dll"));
	m_data_cache_dll	= LoadLibraryA(_("datacache.dll"));
	m_studio_render_dll = LoadLibraryA(_("studiorender.dll"));
	m_server_dll		= LoadLibraryA(_("server.dll"));
}