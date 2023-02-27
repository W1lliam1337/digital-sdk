#ifndef VALVE_BASE_WEAPON
#define VALVE_BASE_WEAPON

#include "entity.hh"

class c_weapon_info {
    [[maybe_unused]] std::byte gap14[ 140 ]{ };
public:
    char* m_weapon_name{ };
private:
    [[maybe_unused]] std::byte gap90[ 48 ]{ };
public:
    int m_weapon_price{ };
    int m_kill_award{ };
    int m_weapon_type{ };
private:
    [[maybe_unused]] std::byte gap6[ 16 ]{ };
public:
    float m_cycle_time{ };
    float m_cycle_time_alt{ };
    float m_time_to_idle{ };
    float m_idle_interval{ };
    bool m_is_full_auto{ };
private:
    [[maybe_unused]] std::byte gapED[ 3 ]{ };
public:
    int m_damage{ };
    float m_headshot_multiplier{ };
    float m_armor_ratio{ };
    int m_bullets{ };
    float m_penetration{ };
    float m_flinch_velocity_modifier_large{ };
    float m_flinch_velocity_modifier_small{ };
    float m_range{ };
    float m_range_modifier{ };
    float m_throw_velocity{ };
private:
   [[maybe_unused]] std::byte pad11[ 12 ]{ };
public:
    bool m_has_silencer{ };
    char* m_silencer_model{ };
private:
   [[maybe_unused]] std::byte pad12[ 4 ]{ };
public:
    int m_crosshair_delta_distance{ };
    float m_max_player_speed{ };
    float m_max_player_speed_alt{ };
    float m_attack_movespeed_factor{ };
    float m_speed{ };
    float m_speed_alt{ };
    float m_inaccuracy_crouch{ };
    float m_inaccuracy_crouch_alt{ };
    float m_inaccuracy_stand{ };
    float m_inaccuracy_stand_alt{ };
    float m_inaccuracy_jump_initial{ };
    float m_inaccuracy_jump_apex{ };
    float m_inaccuracy_jump{ };
    float m_inaccuracy_jump_alt{ };
    float m_inaccuracy_land{ };
    float m_inaccuracy_land_alt{ };
    float m_inaccuracy_ladder{ };
    float m_inaccuracy_ladder_alt{ };
    float m_inaccuracy_fire{ };
    float m_inaccuracy_fire_alt{ };
    float m_inaccuracy_move{ };
    float m_inaccuracy_move_alt{ };
    float m_inaccuracy_reload{ };
    float m_recoil_seed{ };
    float m_recoil_angle{ };
    float m_recoil_angle_alt{ };
    float m_recoil_angle_variance{ };
    float m_recoil_angle_variance_alt{ };
    float m_recoil_magnitude{ };
    float m_recoil_magnitude_alt{ };
    float m_recoil_magnitude_variance{ };
    float m_recoil_magnitude_variance_alt{ };
    float m_spread_seed{ };
    float m_recovery_time_crouch{ };
    float m_recovery_time_stand{ };
    float m_recovery_time_crouch_final{ };
    float m_recovery_time_stand_final{ };
    float m_recovery_transition_start_bullet{ };
    float m_recovery_transition_end_bullet{ };
    bool m_unzoom_after_shot{ };
    bool m_hide_view_model_zoomed{ };
private:
    [[maybe_unused]] std::byte gap1CE[ 2 ]{ };
public:
    int m_zoom_levels{ };
    int m_zoom_fov1{ };
    int m_zoom_fov2{ };
    float m_zoom_time0{ };
    float m_zoom_time1{ };
    float m_zoom_time2{ };
private:
    [[maybe_unused]] std::byte gap1E8[ 8 ]{ };
public:
    float m_addon_scale{ };
private:
    [[maybe_unused]] std::byte gap1F4[ 8 ]{ };
public:
    float m_tracer_frequency{ };
    float m_tracer_frequency_alt{ };
private:
    [[maybe_unused]] std::byte gap204[ 20 ]{ };
public:
    float m_heat_per_shot{ };
private:
    [[maybe_unused]] std::byte gap21C[ 8 ]{ };
public:
    float m_inaccuracy_pitch_shift{ };
    float m_inaccuracy_alt_sound_threshold{ };
    float m_bot_audible_range{ };
private:
    [[maybe_unused]] std::byte gap230[ 12 ]{ };
public:
    bool m_has_burst_mode{ };
    bool m_is_revolver{ };
    bool m_cannot_shoot_underwater{ };
};

class c_attributable_item : public c_entity {
public:
    NETVAR( std::uint64_t, original_owner_xuid, _( "DT_BaseAttributableItem->m_OriginalOwnerXuidLow" ) );
    NETVAR( int, original_owner_xuid_low, _( "DT_BaseAttributableItem->m_OriginalOwnerXuidLow" ) );
    NETVAR( int, original_owner_xuid_high, _( "DT_BaseAttributableItem->m_OriginalOwnerXuidHigh" ) );
    NETVAR( int, fallback_stat_trak, _( "DT_BaseAttributableItem->m_nFallbackStatTrak" ) );
    NETVAR( int, fallback_paint_kit, _( "DT_BaseAttributableItem->m_nFallbackPaintKit" ) );
    NETVAR( int, fallback_seed, _( "DT_BaseAttributableItem->m_nFallbackSeed" ) );
    NETVAR( float, fallback_wear, _( "DT_BaseAttributableItem->m_flFallbackWear" ) );
    NETVAR( bool, is_initialized, _( "DT_BaseAttributableItem->m_bInitialized" ) );
    NETVAR( int, entity_level, _( "DT_BaseAttributableItem->m_iEntityLevel" ) );
    NETVAR( int, account_id, _( "DT_BaseAttributableItem->m_iAccountID" ) );
    NETVAR( int, item_id_low, _( "DT_BaseAttributableItem->m_iItemIDLow" ) );
    NETVAR( int, item_id_high, _( "DT_BaseAttributableItem->m_iItemIDHigh" ) );
    NETVAR( int, entity_quality, _( "DT_BaseAttributableItem->m_iEntityQuality" ) );
    NETVAR( char, custom_name, _( "DT_BaseAttributableItem->m_szCustomName" ) );
    NETVAR( short, idx, _( "DT_BaseAttributableItem->m_iItemDefinitionIndex" ) );
};

class c_weapon : public c_attributable_item {
public:
    VFUNC( void( __thiscall* )( void* ), update_accuracy_penalty( ), 484 );
    VFUNC( float( __thiscall* )( void* ), spread( ), 453 );
    VFUNC( float( __thiscall* )( void* ), inaccuracy( ), 483 );
    VFUNC( c_weapon_info*( __thiscall* ) ( void* ), weapon_data( ), 461 );
};

#endif