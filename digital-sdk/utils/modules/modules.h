#pragma once
#include "../../includes.h"

namespace modules
{
	inline HMODULE m_client_dll{}, m_engine_dll{}, m_vgui_dll{}, m_vgui2_dll{}, m_tier_dll{}, m_mat_sys_dll{}, 
		localize_dll{}, m_shader_dll{}, m_data_cache_dll{}, m_vstd_dll{}, m_physics_dll{}, m_file_system_dll{}, 
		m_server_dll{}, m_studio_render_dll{};

	void init() noexcept;
}