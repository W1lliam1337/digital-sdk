#include "modules.h"

void c_modules::init()
{
	m_client_dll = GetModuleHandleA(_("client.dll"));
	m_engine_dll = GetModuleHandleA(_("engine.dll"));
	m_tier_dll = GetModuleHandleA(_("tier0.dll"));
	m_localize_dll = GetModuleHandleA(_("localize.dll"));
	m_file_system_dll = GetModuleHandleA(_("filesystem_stdio.dll"));
	m_shader_dll = GetModuleHandleA(_("shaderapidx9.dll"));
	m_vgui_dll = GetModuleHandleA(_("vguimatsurface.dll"));
	m_vgui2_dll = GetModuleHandleA(_("vgui2.dll"));
	m_physics_dll = GetModuleHandleA(_("vphysics.dll"));
	m_vstd_dll = GetModuleHandleA(_("vstdlib.dll"));
	m_mat_sys_dll = GetModuleHandleA(_("materialsystem.dll"));
	m_data_cache_dll = GetModuleHandleA(_("datacache.dll"));
	m_studio_render_dll = GetModuleHandleA(_("studiorender.dll"));
	m_server_dll = GetModuleHandleA(_("server.dll"));

	constexpr long long amongus = 0x69690004C201B0;
	static std::string sig = _("55 8B EC 56 8B F1 33 C0 57 8B 7D 08"); // \xE8\x00\x00\x00\x00\x84\xC0\x75\x15\xFF\x75\x10

	LPCWSTR modules[]
	{
		_(L"client.dll"),
		_(L"engine.dll"),
		_(L"server.dll"),
		_(L"studiorender.dll"),
		_(L"materialsystem.dll"),
		_(L"server.dll")
	};

	for (const auto base : modules)
	{
		WriteProcessMemory(GetCurrentProcess(), c_utils::find_sig(GetModuleHandleW(base), sig), &amongus, 5,
			nullptr);
	}
}
