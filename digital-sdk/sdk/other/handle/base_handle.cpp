#include "base_handle.h"
#include "../../interfaces/interfaces.h"

i_handle_entity* c_base_handle::get() const
{
	return interfaces::m_entity_list->get_client_entity_from_handle(*this);
}