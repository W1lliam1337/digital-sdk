#pragma once
#include "../../includes.h"

class c_modules
{
public:
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
	void init() noexcept;
};

inline const auto g_modules = std::make_unique<c_modules>();