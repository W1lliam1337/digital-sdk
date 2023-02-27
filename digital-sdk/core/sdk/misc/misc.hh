#ifndef VALVE_MISC_H
#define VALVE_MISC_H

#include "base_handle.hh"
#include "client_class.hh"
#include "client_entity.hh"
#include "client_state.hh"
#include "collideable.hh"
#include "data_map.hh"
#include "dt_recv.hh"
#include "game_trace.hh"
#include "key_values.hh"
#include "studio.hh"
#include "user_cmd.hh"
#include "bone_accessor.hh"
#include "ray.hh"

#define to_ticks( time ) static_cast< int >( 0.5f + time / g_interfaces->m_globals->m_interval_per_tick );
#define to_time( tick ) static_cast< float >( tick ) * g_interfaces->m_globals->m_interval_per_tick;

#endif