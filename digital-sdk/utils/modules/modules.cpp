#include "modules.h"

void modules::init() noexcept
{
	/* @xref: https://docs.microsoft.com/en-us/windows/win32/dlls/run-time-dynamic-linking */
	m_client_dll		= LoadLibraryA(_("client.dll"));
	m_engine_dll		= LoadLibraryA(_("engine.dll"));
	m_tier_dll			= LoadLibraryA(_("tier0.dll"));
	localize_dll		= LoadLibraryA(_("localize.dll"));
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

	printf(" client_dll: 0x%p\n engine.dll: 0x%p\n tier.dll: 0x%p\n localize.dll: 0x%p\n file_system.dll: 0x%p\n shader.dll: 0x%p\n vgui.dll: 0x%p\n vgui2.dll: 0x%p\n physics.dll: 0x%p\n vstd.dll: 0x%p\n mat_sys.dll: 0x%p\n data_cache.dll: 0x%p\n studio_render.dll: 0x%p\n server.dll: 0x%p\n",
		m_client_dll, m_engine_dll, m_tier_dll, localize_dll, m_file_system_dll, m_shader_dll, m_vgui_dll, m_vgui2_dll, m_physics_dll, m_vstd_dll, m_mat_sys_dll, m_data_cache_dll, m_studio_render_dll, m_server_dll);
}