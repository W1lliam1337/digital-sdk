#pragma once
#include "entity.h"

class c_client_precipitation : public c_base_entity {
public:
	GET_OFFSET( e_precipitation_type, get_precip_type, 0xA00 );
	GET_OFFSET( bool, get_particle_recip_initialized, 0xAA1 );
};