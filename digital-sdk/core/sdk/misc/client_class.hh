#ifndef VALVE_CLIENT_CLASS
#define VALVE_CLIENT_CLASS

#include "dt_recv.hh"

using create_client_class_fn = void* ( * ) ( int, int );
using create_event_fn = void* ( * ) ( );

class c_client_class {
public:
    create_client_class_fn m_create_fn{ };
    create_event_fn m_create_event_fn{ };
    char* m_network_name{ };
    recv_table_t* m_recv_table{ };
    c_client_class* m_next{ };
    int m_class_id{ };
    const char* m_map_classname{ };
};

#endif