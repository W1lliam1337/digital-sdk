#pragma once
#include "classes.h"
#include "../interfaces/classes/i_client_entity.h"
#include "../../utils/netvar-manager/netvar_manager.h"
#include "../../utils/utils.h"
#include "enums.h"

class c_base_entity : public i_client_entity
{
public:
	GET_NETVAR(int, get_team, _("DT_BaseEntity"), _("m_iTeamNum"));
	GET_NETVAR(int, get_collision_group, _("DT_BaseEntity"), _("m_CollisionGroup"));
	GET_NETVAR_OFFSET(e_move_type, get_move_type, _("DT_BaseEntity"), _("m_nRenderMode"), 0x1);
	GET_NETVAR_OFFSET(const matrix_t, get_coordinate_frame, _("DT_BaseEntity"), _("m_CollisionGroup"), -0x30);
};
