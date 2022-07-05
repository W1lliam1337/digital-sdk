#pragma once
#include "../../includes.h"

enum e_module_list : int
{
	client_dll,
	engine_dll,
	tier_dll,
	localize_dll,
	file_system_dll,
	shader_dll,
	vgui_dll,
	vgui2_dll,
	physics_dll,
	vstd_dll,
	mat_sys_dll,
	data_cache_dll,
	studio_render_dll,
	server_dll
};

class c_modules
{
public:
	std::vector<HMODULE> m_modules{};
	HMODULE m_client_dll{};
	HMODULE m_engine_dll{};
	HMODULE m_vgui_dll{};
	HMODULE m_vgui2_dll{};
	HMODULE m_tier_dll{};
	HMODULE m_mat_sys_dll{};
	HMODULE m_localize_dll{};
	HMODULE m_shader_dll{};
	HMODULE m_data_cache_dll{};
	HMODULE m_vstd_dll{};
	HMODULE m_physics_dll{};
	HMODULE m_file_system_dll{};
	HMODULE m_server_dll{};
	HMODULE m_studio_render_dll{};
	void init();
};

inline c_modules g_modules;