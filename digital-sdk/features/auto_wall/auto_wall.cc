#include "auto_wall.hh"

void c_auto_wall::scale_damage( const c_game_trace& enter_trace, const c_weapon_info* weapon_info, float& dmg ) const {
    // @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/server/player.cpp#L1194
    static auto mp_damage_scale_ct_head = g_interfaces->m_cvar->find_var( _( "mp_damage_scale_ct_head" ) );
    static auto mp_damage_scale_t_head = g_interfaces->m_cvar->find_var( _( "mp_damage_scale_t_head" ) );

    static auto mp_damage_scale_ct_body = g_interfaces->m_cvar->find_var( _( "mp_damage_scale_ct_body" ) );
    static auto mp_damage_scale_t_body = g_interfaces->m_cvar->find_var( _( "mp_damage_scale_t_body" ) );

     auto head_damage_scale = enter_trace.m_entity->team( ) == 3
        ? mp_damage_scale_ct_head->get_float( ) : enter_trace.m_entity->team( ) == 2
        ? mp_damage_scale_t_head->get_float( ) : 1.0f;

    const auto body_damage_scale = enter_trace.m_entity->team( ) == 3
        ? mp_damage_scale_ct_body->get_float( ) : enter_trace.m_entity->team( ) == 2
        ? mp_damage_scale_t_body->get_float( ) : 1.0f;

    if ( enter_trace.m_entity->is_heavy_armor( ) )
        head_damage_scale *= 0.5f;

    switch ( enter_trace.m_hit_group ) {
        case hitgroup_head:
            dmg *= weapon_info->m_headshot_multiplier * head_damage_scale;
            break;
        case hitgroup_chest:
        case hitgroup_leftarm:
        case hitgroup_rightarm:
        case hitgroup_neck:
            dmg *= body_damage_scale;
            break;
        case hitgroup_stomach:
            dmg *= 1.25f * body_damage_scale;
            break;
        case hitgroup_leftleg:
        case hitgroup_rightleg:
            dmg *= 0.75f * body_damage_scale;
            break;
        default:
            break;
    }

    if ( !enter_trace.m_entity->is_armored( enter_trace.m_hit_group ) )
        return;

    // @ida: module: server.dll; sig: 80 BF ? ? ? ? ? F3 0F 10 5C 24 ? F3 0F 10 35
    float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = weapon_info->m_armor_ratio * 0.5f;

    if ( enter_trace.m_entity->is_heavy_armor( ) ) {
        heavy_armor_bonus = 0.25f;
        armor_bonus = 0.33f;
        armor_ratio *= 0.20f;
    }

    float damage_to_health = dmg * armor_ratio;
    const float damage_to_armor = ( dmg - damage_to_health ) * ( heavy_armor_bonus * armor_bonus );

    if ( damage_to_armor > static_cast<float>( enter_trace.m_entity->armor_value( ) ) )
        damage_to_health = dmg - static_cast<float>( enter_trace.m_entity->armor_value( ) ) / armor_bonus;

    dmg = damage_to_health;
}

bool c_auto_wall::trace_to_exit( const c_game_trace& enter_trace, c_game_trace& exit_trace, vec3_t start,
                                 vec3_t direction ) const {
    // @ida: https://imgur.com/x3Qe12r
    // module: server.dll; sig: F3 0F 5C CE F3 0F 11 5D ?

    auto distance = 0.0f;
    int v13{ };
    vec3_t end{ };

    while ( distance <= 90.0f ) {
        distance += 4.0f;

        end = start + direction * distance;

        if ( !v13 )
            v13 = g_interfaces->m_trace->get_point_contents( end, 0x4600400B, nullptr );

        vec3_t cl_end = end - direction * 4.0f;

        const auto contents = g_interfaces->m_trace->get_point_contents( end, 0x4600400B, nullptr );
        if ( contents & 0x600400B && ( contents & 0x40000000 || v13 == contents ) )
            continue;

        static const auto trace_filter = g_modules->m_client_dll.get_address( _( "TraceFilter" ) ).offset( 0x3D ).as< std::uint32_t* >( );

        const std::uint32_t filter[ 4 ] = { *trace_filter, reinterpret_cast< std::uint32_t >( exit_trace.m_entity ), 0,
                                            0 };

        g_interfaces->m_trace->trace_ray( ray_t( end, cl_end ), 0x4600400B, nullptr, &exit_trace );

        if ( static auto var = g_interfaces->m_cvar->find_var( _( "sv_clip_penetration_traces_to_players" ) );
             var->get_int( ) ) {
            clip_trace_to_players( end, cl_end, ( c_trace_filter* ) ( filter ), &exit_trace, -60.0f );
        }

        if ( exit_trace.m_start_solid && exit_trace.m_surface.m_flags & surf_hitbox ) {
            g_interfaces->m_trace->trace_ray( ray_t( end, start ), 0x600400B, ( c_trace_filter* ) ( filter ),
                                              &exit_trace );

            if ( exit_trace.did_hit( ) && !exit_trace.m_start_solid ) {
                end = exit_trace.m_end;
                return true;
            }

            continue;
        }

        if ( exit_trace.did_hit( ) && !exit_trace.m_start_solid ) {
            if ( enter_trace.m_entity->is_breakable( ) && exit_trace.m_entity->is_breakable( ) )
                return true;

            if ( enter_trace.m_surface.m_flags & surf_nodraw
                 || !( exit_trace.m_surface.m_flags & surf_nodraw )
                     && exit_trace.m_plane.m_normal.dot( direction ) <= 1.0f ) {
                start -= direction * exit_trace.m_fraction * 4.0f;
                return true;
            }

            continue;
        }

        if ( !exit_trace.did_hit( ) || exit_trace.m_start_solid ) {
            if ( enter_trace.m_entity != nullptr && enter_trace.m_entity->get_index( ) > 0
                 && enter_trace.m_entity->is_breakable( ) ) {
                exit_trace = enter_trace;
                exit_trace.m_end = start + direction;
                return true;
            }
        }
    }

    return false;
}

void c_auto_wall::clip_trace_to_players( const vec3_t& abs_start, const vec3_t& abs_end, c_trace_filter* filter,
                                         c_game_trace* trace, const float min_range ) const {
    static const auto clip_trace_to_players_fn = g_modules->m_client_dll.get_address( _( "ClipTraceToPlayers" ) ).as<
        void( __thiscall* )( c_handle_entity*, vec3_t, vec3_t, unsigned int, i_trace_filter*, c_game_trace*, float )>( );

    return clip_trace_to_players_fn( c_player::get_local( ), abs_start, abs_end, mask_shot_hull | contents_hitbox,
                                     filter, trace, min_range );
}

bool c_auto_wall::handle_bullet_penetration( const c_weapon_info* weapon_data, const c_game_trace& enter_trace,
                                             const vec3_t& direction, vec3_t& eye_pos, int& hits, float& current_dmg,
                                             const float pen_power ) const {
    // @ida:
    // module: server.dll; sig: 39 07 75 1C;
    // module: client.dll: sig: E8 ? ? ? ? 83 C4 40 84 C0;

    c_game_trace exit_trace;

    const auto enter_surface_data =
        g_interfaces->m_phys_surface_props->get_surface_data( enter_trace.m_surface.m_surface_props );
    const auto enter_material = enter_surface_data->m_game.m_material;

    const bool is_solid_surf = enter_trace.m_contents >> 3 & contents_solid;
    const bool is_light_surf = enter_trace.m_surface.m_flags >> 7 & surf_light;

    const auto v85 = ( enter_trace.m_contents & 8 ) != 0;
    const auto v86 = ( enter_trace.m_surface.m_flags & 0x80 ) != 0;

    if ( !hits && !v85 && !v86 && enter_material != 89 && enter_material != 71 )
        return false;

    if ( !weapon_data->m_penetration
         || !trace_to_exit( enter_trace, exit_trace, enter_trace.m_end, direction )
         && !( g_interfaces->m_trace->get_point_contents( enter_trace.m_end, 0x600400B, nullptr ) & 0x600400B ) )
        return false;

    static auto var = g_interfaces->m_cvar->find_var( _( "sv_penetration_type" ) );
    const int possible_hits = var->get_int( );

    const auto exit_surface_data =
        g_interfaces->m_phys_surface_props->get_surface_data( exit_trace.m_surface.m_surface_props );
    const auto exit_material = exit_surface_data->m_game.m_material;

    static auto ff_damage_bullet_penetration = g_interfaces->m_cvar->find_var( _( "ff_damage_bullet_penetration" ) );
    static auto ff_damage_reduction_bullets = g_interfaces->m_cvar->find_var( _( "ff_damage_reduction_bullets" ) );

    float combined_penetration_modifier{ };
    float final_damage_modifier;

    if ( possible_hits != 1 ) {
        if ( is_solid_surf || is_light_surf ) {
            combined_penetration_modifier = 1.0f;
            final_damage_modifier = 0.99f;
        } else {
            final_damage_modifier = enter_surface_data->m_game.m_dmg_modifier;
            combined_penetration_modifier =
                std::fminf( enter_surface_data->m_game.m_pen_modifier, exit_surface_data->m_game.m_pen_modifier );
            if ( enter_surface_data->m_game.m_dmg_modifier > enter_surface_data->m_game.m_jump_factor )
                final_damage_modifier = enter_surface_data->m_game.m_jump_factor;
        }

        if ( enter_material == exit_material && ( exit_material == 87 || exit_material == 77 ) )
            combined_penetration_modifier *= 2.0f;

        if ( ( exit_trace.m_end - enter_trace.m_end ).length_sqr( ) > combined_penetration_modifier * pen_power )
            return false;

        current_dmg *= final_damage_modifier;
        eye_pos = exit_trace.m_end;
        --hits;

        return true;
    }

    final_damage_modifier = 0.16f;
    if ( !is_solid_surf && !is_light_surf ) {
        if ( enter_material == 89 ) {
            final_damage_modifier = 0.05f;
            combined_penetration_modifier = 3.0f;
        }

        if ( enter_material != 71 ) {
            // client.dll
            const auto hit_ent = enter_trace.m_entity;
            if ( hit_ent ) {
                if ( hit_ent->is_player( ) && hit_ent->unk_thing( ) ) {
                    combined_penetration_modifier = exit_surface_data->m_game.m_pen_modifier;
                    final_damage_modifier = 0.16f;
                }
            }

            if ( enter_material == 70 && ( ff_damage_reduction_bullets->get_float( ) == 0.0f && hit_ent ) ) {
                // mp_teammates_are_enemies
                const auto is_enemy = hit_ent != c_player::get_local( ) && hit_ent->team( ) != c_player::get_local( )->team( );

                if ( hit_ent->is_player( ) && !is_enemy ) {
                    if ( ff_damage_bullet_penetration->get_float( ) == 0.0f )
                        return false;

                    combined_penetration_modifier = ff_damage_bullet_penetration->get_float( );
                    final_damage_modifier = 0.16f;
                }
            }

            combined_penetration_modifier = ( enter_surface_data->m_game.m_pen_modifier + exit_surface_data->m_game.m_pen_modifier ) / 2.0f;
            final_damage_modifier = 0.16f;
        }
    }

    if ( enter_material != 89 && enter_material != 71 ) {
        combined_penetration_modifier = 1.0f;
        if ( enter_material == exit_material ) {
            if ( exit_material == 87 || exit_material == 85 ) {
                combined_penetration_modifier = 3.0f;
            } else if ( exit_material == 76 ) {
                combined_penetration_modifier = 2.0f;
            }
        }
    }

    const auto thickness = ( exit_trace.m_end - enter_trace.m_end ).length_sqr( );
    const auto modifier = std::fmax( 1.0f / combined_penetration_modifier, 0.0f );

    const auto lost_damage = std::fmax( modifier * thickness / 24.0f + current_dmg * final_damage_modifier
                                            + std::fmax( 3.75f / pen_power, 0.0f ) * 3.0f * modifier,
                                        0.0f );

    if ( lost_damage > current_dmg )
        return false;

    if ( lost_damage > 0.0f )
        current_dmg -= lost_damage;

    if ( current_dmg < 1.0f )
        return false;

    eye_pos = exit_trace.m_end;
    --hits;

    return true;
}

bool c_auto_wall::fire_bullet( c_player* const player, const vec3_t& direction, float& current_dmg, const vec3_t& point,
                               vec3_t& eye_pos, int& hits ) const {
    const auto weapon = c_player::get_local( )->active_weapon( );
    if ( !weapon )
        return false;

    const auto weapon_data = weapon->weapon_data( );
    if ( !weapon_data )
        return false;

    c_game_trace enter_trace;
    static const auto trace_filter = g_modules->m_client_dll.get_address( _( "TraceFilter" ) ).offset( 0x3D ).as< std::uint32_t* >( );
    const std::uint32_t filter[ 4 ] = { *trace_filter, reinterpret_cast< std::uint32_t >( c_player::get_local( ) ), 0,
                                        0 };

    current_dmg = static_cast<float>( weapon_data->m_damage );
    constexpr auto penetration_distance = 3000.0f;
    auto penetration_power = 35.0f;
    hits = 4;

    static auto var = g_interfaces->m_cvar->find_var( _( "sv_penetration_type" ) );
    if ( const int possible_hits = var->get_int( ); possible_hits == 1 ) {
        penetration_power = weapon_data->m_penetration;
    }

    const float max_range = min( weapon_data->m_range, ( eye_pos - point ).length( ) );
    const auto end = eye_pos + direction * max_range;

    while ( current_dmg >= 1.0f ) {
        g_interfaces->m_trace->trace_ray( ray_t( eye_pos, end ), mask_shot_hull | contents_hitbox,
                                          ( c_trace_filter* ) filter, &enter_trace );

        if ( player )
            clip_trace_to_players( eye_pos, end + direction * 40.0f, ( c_trace_filter* ) filter, &enter_trace, 0.0f );

        const auto enter_surface_data =
            g_interfaces->m_phys_surface_props->get_surface_data( enter_trace.m_surface.m_surface_props );
        const auto enter_surf_penetration_modifier = enter_surface_data->m_game.m_pen_modifier;

        const float distance_traced = ( enter_trace.m_end - eye_pos ).length( );
        current_dmg *= std::pow( weapon_data->m_range_modifier, distance_traced / 500.f );

        if ( enter_trace.m_fraction == 1.0f ) {
            break;
        }

        if ( distance_traced > penetration_distance && weapon_data->m_penetration > 0.0f
             || enter_surf_penetration_modifier < 0.1f ) {
            break;
        }

        // if ( *( v123 + 13196 ) == v83 ) {
        //	v115 = 4352;
        //	if ( *( hit_ent + 39649 ) )
        //		current_damage = 1.0;
        // }

        const auto can_do_damage = enter_trace.m_hit_group != hitgroup_gear && enter_trace.m_hit_group != hitgroup_generic;
        const auto is_player = enter_trace.m_entity->is_player( );
        const auto is_enemy = enter_trace.m_entity->team( ) != c_player::get_local( )->team( );

        if ( can_do_damage && is_player && is_enemy ) {
            scale_damage( enter_trace, weapon_data, current_dmg );
            return true;
        }

        if ( !hits )
            break;

        if ( !handle_bullet_penetration( weapon_data, enter_trace, direction, eye_pos, hits, current_dmg,
                                         penetration_power ) )
            break;
    }

    return false;
}