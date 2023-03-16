#ifndef VALVE_BASE_ENTITY
#define VALVE_BASE_ENTITY

#include "../net_vars/net_vars.hh"
#include "../../utils/utils.hh"
#include "../modules/modules.hh"
#include "../misc/client_entity.hh"
#include "../misc/user_cmd.hh"

enum e_move_type {
    move_type_none = 0,
    move_type_isometric,
    move_type_walk,
    move_type_step,
    move_type_fly,
    move_type_fly_gravity,
    move_type_vphysics,
    move_type_push,
    move_type_noclip,
    move_type_ladder,
    move_type_ob_server,
    move_type_custom,
    move_type_last = move_type_custom,
    move_type_max_bits = 4
};

enum e_flags {
    fl_on_ground = 1 << 0,
    fl_ducking = 1 << 1,
    fl_anim_ducking = 1 << 2,
    fl_water_jump = 1 << 3,
    fl_on_train = 1 << 4,
    fl_in_rain = 1 << 5,
    fl_frozen = 1 << 6,
    fl_at_controls = 1 << 7,
    fl_client = 1 << 8,
    fl_fake_client = 1 << 9,
    fl_in_water = 1 << 10,
    fl_fly = 1 << 11,
    fl_swim = 1 << 12,
    fl_conveyor = 1 << 13,
    fl_npc = 1 << 14,
    fl_god_mode = 1 << 15,
    fl_no_target = 1 << 16,
    fl_aim_target = 1 << 17,
    fl_partial_ground = 1 << 18,
    fl_static_prop = 1 << 19,
    fl_graphed = 1 << 20,
    fl_grenade = 1 << 21,
    fl_step_movement = 1 << 22,
    fl_dont_touch = 1 << 23,
    fl_base_velocity = 1 << 24,
    fl_world_brush = 1 << 25,
    fl_object = 1 << 26,
    fl_kill_me = 1 << 27,
    fl_on_fire = 1 << 28,
    fl_dissolving = 1 << 29,
    fl_trans_ragdoll = 1 << 30,
    fl_unblockable_by_player = 1 << 31
};

enum e_damage { damage_no, damage_events_only, damage_yes, damage_aim };

enum e_hitgroups {
    hitgroup_generic = 0,
    hitgroup_head,
    hitgroup_chest,
    hitgroup_stomach,
    hitgroup_leftarm,
    hitgroup_rightarm,
    hitgroup_leftleg,
    hitgroup_rightleg,
    hitgroup_neck,
    hitgroup_gear = 10
};

enum e_invalidate_physics_bits {
    position_changed = 0x1,
    angles_changed = 0x2,
    velocity_changed = 0x4,
    animation_changed = 0x8,
    bounds_changed = 0x10,
    sequence_changed = 0x20,
};

class c_entity : public c_client_entity {
public:
    NETVAR( int, team, _( "DT_BaseEntity->m_iTeamNum" ) );
    NETVAR( vec3_t, origin, _( "DT_BaseEntity->m_vecOrigin" ) );
    NETVAR_OFFSET( e_move_type, move_type, _( "DT_BaseEntity->m_nRenderMode" ), 0x1 );

    OFFSET( int, take_damage, 0x280 );

    VFUNC( data_map_t*( __thiscall* ) ( void* ), get_data_desc_map( ), 15 );
    VFUNC( data_map_t*( __thiscall* ) ( void* ), get_pred_desc_map( ), 17 );

    static void set_pred_random_seed( const c_user_cmd* cmd ) {
        static const auto sig = g_modules->m_client_dll.get_address( _( "C_BaseEntity::PredictionRandomSeed" ) ).offset( 0x5 ).to< int* >( );

        if ( !cmd || !cmd->m_random_seed ) {
            *sig = -1;
            return;
        }

        *sig = cmd->m_random_seed;
    }

    void set_pred_player( c_entity* player ) {
        static const auto sig = g_modules->m_client_dll.get_address( _( "C_BaseEntity::PredictionPlayer" ) ).offset( 0x4 ).to< c_entity** >( );
        *sig = player;
    }

    bool physics_run_think( const int index = 0 /* THINK_FIRE_ALL_FUNCTIONS */ ) {
        static const auto sig = g_modules->m_client_dll.get_address( _( "C_BaseEntity::PhysicsRunThink" ) ).as< 
            bool( __thiscall* )( void*, int ) >( );
        return sig( this, index );
    }

    void unk_think_func( const int a1 ) {
        static const auto sig = g_modules->m_client_dll.get_address( _( "C_BaseEntity::UnkThinkFunc" ) ).as< 
            void( __thiscall* )( void*, int ) >( );
        return sig( this, a1 );
    }

    bool post_think_v_physics( ) {
        static const auto sig = g_modules->m_client_dll.get_address( _( "C_BaseEntity::PostThinkVPhysics" ) ).as< 
            bool( __thiscall* )( void* ) >( );
        return sig( this );
    }

    void simulate_player_simulated_entities( ) {
        static const auto sig = g_modules->m_client_dll.get_address( _( "C_BaseEntity::SimulatePlayerSimulatedEntities" ) ).as< 
            void( __thiscall* )( void* ) >( );
        return sig( this );
    }

    bool is_breakable( ) {
        static const auto is_breakable = g_modules->m_client_dll.get_address( _( "C_BaseEntity::IsBreakableEntity" ) ).as< 
            bool( __thiscall* )( void* ) >( );

        const auto backup_take_damage = take_damage( );

        const auto client_class = get_client_class( );
        const auto network_name = client_class->m_network_name;

        // CBreakableSurface -> func_breakable_surf
        // CBreakable -> func_breakable
        if ( !strcmp( network_name, _( "CBreakableSurface" ) ) || !strcmp( network_name, _( "CBreakable" ) ) ) {
            take_damage( ) = damage_yes;
        }

        bool result = is_breakable( this );

        take_damage( ) = backup_take_damage;

        return result;
    }

    // @ida: module: client.dll; sig: 56 8B F1 8B 06 FF 90 ?? ?? ?? ?? 83 F8 01 75 04
    // bool is_bsp_model( ) {
    //	if ( get_solid( ) == 1 )
    //		return true;

    //	static const auto index = static_cast< int >( reinterpret_cast< std::uintptr_t >( this ) + 0x258 );
    //	const auto v3 = g_interfaces->m_model_info->get_model( index );
    //	const auto v2 = g_interfaces->m_model_info->get_model_type( v3 );

    //	if ( get_solid( ) != 6 || v2 != 1 )
    //		return true;

    //	return false;
    //}
};

#endif