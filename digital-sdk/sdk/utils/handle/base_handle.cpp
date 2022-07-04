#include "base_handle.h"
#include "../../sdk.hpp"

i_handle_entity* c_base_handle::get() const
{
	if (!g_sdk.m_interfaces.m_entity_list)
		return nullptr;

	return g_sdk.m_interfaces.m_entity_list->get_client_entity_from_handle (*this);
}