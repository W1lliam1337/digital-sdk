#ifndef VALVE_BASE_CDLL_CLIENT_INIT
#define VALVE_BASE_CDLL_CLIENT_INIT

#include "../../../utils/memory/memory.hh"
#include "../../misc/client_class.hh"

class i_base_client_dll {
public:
    VFUNC( c_client_class*( __thiscall* ) ( void* ), get_all_classes( ), 8 );
};

#endif