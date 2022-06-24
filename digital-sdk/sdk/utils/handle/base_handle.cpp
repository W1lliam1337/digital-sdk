#include "base_handle.h"
#include "../../sdk.hpp"

i_handle_entity* c_base_handle::get()
{
	if (!g_sdk.m_interfaces.m_entity_list || !g_sdk.m_interfaces.m_engine)
		return nullptr;

	return g_sdk.m_interfaces.m_entity_list->get_client_entity(g_sdk.m_interfaces.m_engine->get_local_player());
}