#include "../hooks.h"

void __fastcall hooks::hk_override_view(void* ecx, void* edx, c_view_setup* setup_view)
{
	if (!ctx::local() || !ctx::local()->is_alive())
	{
		interfaces::m_input->m_camera_in_third_person = false;
		return;
	}

	if (!interfaces::m_input->m_camera_in_third_person && utils::is_bind_active(g_cfg.m_misc.m_third_person_bind))
		interfaces::m_input->m_camera_in_third_person = true;
	else
	{
		if (!utils::is_bind_active(g_cfg.m_misc.m_third_person_bind))
			interfaces::m_input->m_camera_in_third_person = false;
	}

	interfaces::m_input->m_camera_offset.z = static_cast<float>(g_cfg.m_misc.m_third_person_distance);
	return og::m_override_view(ecx, edx, setup_view);
}
