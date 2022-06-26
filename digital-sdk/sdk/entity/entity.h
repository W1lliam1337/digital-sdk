#pragma once
#include "../sdk.hpp"
#include "../utils/netvar manager/netvar_manager.h"

class c_weapon;

inline uint32_t find_in_data_map(c_data_map* data_map, const std::string& prop_name)
{
	while (data_map)
	{
		for (int i = 0; i < data_map->m_data_num_fields; i++)
		{
			if (!data_map->m_data_description[i].field_name)
				continue;

			if (data_map->m_data_description[i].field_name == prop_name)
				return data_map->m_data_description[i].field_offset[td_offset_normal];

			if (data_map->m_data_description[i].field_type != field_embedded)
				continue;

			if (!data_map->m_data_description[i].td)
				continue;

			const uint32_t iOffset = find_in_data_map(data_map->m_data_description[i].td, prop_name);
			if (!iOffset)
				continue;

			return iOffset;
		}

		data_map = data_map->m_base_map;
	}

	return 0;
}

#define GET_VFUNC( strFunctionName, nIndex, Type, ...) \
auto strFunctionName { \
return call_vfunc< Type >( this, nIndex )( this, __VA_ARGS__ ); \
};

#define GET_DATAMAP( type, name ) \
__forceinline type& name( ) \
{\
	static uint32_t g_Datamap_##name = 0;\
    if ( !g_Datamap_##name )\
        g_Datamap_##name = find_in_data_map( this->get_pred_desc_map( ), #name );\
    return *( type* )( ( uintptr_t )( this ) + g_Datamap_##name );\
}

class c_base_entity : public i_client_entity
{
public:
	GET_VFUNC(get_pred_desc_map(), 17, c_data_map* (__thiscall*)(void*));
	GET_NETVAR(float, m_last_shot_time, _("DT_WeaponCSBase"), _("m_fLastShotTime"));
	GET_NETVAR(int, get_team, _("DT_BaseEntity"), _("m_iTeamNum"));
	GET_NETVAR(int, get_collision_group, _("DT_BaseEntity"), _("m_CollisionGroup"));
	GET_NETVAR(c_base_handle, get_owner_entity, _("DT_BaseEntity"), _("m_hOwnerEntity"));
	GET_NETVAR(vec3_t, get_origin, _("DT_BaseEntity"), _("m_vecOrigin"));
	GET_NETVAR(vec3_t, get_angles, _("DT_BaseEntity"), _("m_vecAngles"));
	GET_NETVAR(bool, get_spotted, _("DT_BaseEntity"), _("m_bSpotted"));
	GET_NETVAR(int, get_model_index, _("DT_BaseEntity"), _("m_nModelIndex"));
	GET_NETVAR(float, get_simulation_time, _("DT_BaseEntity"), _("m_flSimulationTime"));
	GET_NETVAR_OFFSET(float, get_old_simulation_time, _("DT_BaseEntity"), _("m_flSimulationTime"), 4);
	GET_NETVAR_OFFSET(const matrix_t, get_coordinate_frame, _("DT_BaseEntity"), _("m_CollisionGroup"), -0x30);
	GET_NETVAR_OFFSET(e_move_type, get_move_type, _("DT_BaseEntity"), _("m_nRenderMode"), 0x1);
};

class c_base_player : public c_base_entity
{
public:
	//GET_VFUNC(get_shoot_pos(), 285, vec3_t(__thiscall*)(void*));
	//GET_VFUNC(get_eye_pos(), 277, vec3_t(__thiscall*)(void*));
	GET_NETVAR(int, get_impulse, _("DT_CSPlayer"), _("m_nImpulse"));
	GET_NETVAR(c_handle < c_base_player >, get_vehicle, _("DT_BasePlayer"), _("m_hVehicle"));
	GET_NETVAR(int, get_life_state, _("DT_BasePlayer"), _("m_lifeState"));
	GET_NETVAR(float, get_duck_amount, _("DT_BasePlayer"), _("m_flDuckAmount"));
	GET_NETVAR(float, get_lby, _("DT_BasePlayer"), _("m_flLowerBodyYawTarget"));
	GET_NETVAR(bit_flag_t<int>, get_flags, _("DT_BasePlayer"), _("m_fFlags"));
	GET_NETVAR(bool, is_defusing, _("DT_BasePlayer"), _("m_bIsDefusing"));
	GET_NETVAR(int, get_sequence, _("DT_BasePlayer"), _("m_nSequence"));
	GET_NETVAR(int, get_health, _("DT_BasePlayer"), _("m_iHealth"));
	GET_NETVAR(int, get_tick_base, _("DT_BasePlayer"), _("m_nTickBase"));
	GET_NETVAR(bit_flag_t<int>, get_effects, _("DT_BasePlayer"), _("m_fEffects"));
	GET_NETVAR(int, get_observer_mode, _("DT_BasePlayer"), _("m_iObserverMode"));
	GET_NETVAR(float, get_fall_velocity, _("DT_BasePlayer"), _("m_flFallVelocity"));
	GET_NETVAR(vec3_t, get_view_origin, _("DT_BasePlayer"), _("m_vecViewOffset[0]"));
	GET_NETVAR(vec3_t, get_velocity, _("DT_BasePlayer"), _("m_vecVelocity[0]"));
	GET_NETVAR(qangle_t, get_view_punch, _("DT_BasePlayer"), _("m_viewPunchAngle"));
	GET_NETVAR(qangle_t, get_punch, _("DT_BasePlayer"), _("m_aimPunchAngle"));
	GET_NETVAR(qangle_t, get_eye_angles, _("DT_CSPlayer"), _("m_angEyeAngles[0]"));
	GET_NETVAR(c_handle < c_weapon >, get_active_weapon_handle, _("DT_BaseCombatCharacter"), _("m_hActiveWeapon"));
	GET_NETVAR(bool, has_heavy_armor, _("DT_CSPlayer"), _("m_bHasHeavyArmor"));
	GET_NETVAR(bool, has_helmet, _("DT_CSPlayer"), _("m_bHasHelmet"));
	GET_NETVAR(int, get_armour_value, _("DT_CSPlayer"), _("m_ArmorValue"));
	GET_NETVAR(bool, get_client_side_animation, _("DT_BaseAnimating"), _("m_bClientSideAnimation"));
	GET_OFFSET(get_take_damage, int, 0x280);
	GET_OFFSET(get_bone_accessor, c_bone_accressor, 0x26A8);
	GET_OFFSET(get_bone_cache, CUtlVector<matrix_t>, 0x2914)

	c_weapon* get_active_weapon() const;
	void invalidate_bone_cache();
	vec3_t& get_abs_origin();
	bool is_weapon();
	bool is_player();
	unsigned int physics_solid_mask_for_entity();
	void get_think();
	void get_pre_think();
	void set_sequence(int flag);
	void studio_frame_advance();
	void update_collistion_bounds();
	void update_client_side_animations();
	vec3_t& get_eye_pos();
	int is_max_health();
	const char* get_classname();
	void invalidate_physics_recursive(int flag);
	void select_item(const char* string, int sub_type);
	void physics_simulated_entites();
	void post_think();
	c_animstate* get_anim_state();
	animlayer_t* get_anim_layers();
	void set_abs_origin(vec3_t position);
	int get_sequence_activity(int sequence);
	bool using_standard_weapons_in_vehicle();
	bool is_breakable();
	bool post_think_v_physics();
	bool is_alive();
	bool physics_run_think(int index);
	vec3_t get_shoot_pos();
	vec3_t get_hitbox_pos(int hitbox_id);
};

class c_base_attributable_item : public c_base_entity
{
public:
	GET_NETVAR(uint64_t, get_original_owner_xuid, _("DT_BaseAttributableItem"), _("m_OriginalOwnerXuidLow"));
	GET_NETVAR(int, get_original_owner_xuid_low, _("DT_BaseAttributableItem"), _("m_OriginalOwnerXuidLow"));
	GET_NETVAR(int, get_original_owner_xuid_high, _("DT_BaseAttributableItem"), _("m_OriginalOwnerXuidHigh"));
	GET_NETVAR(int, get_fallback_stat_trak, _("DT_BaseAttributableItem"), _("m_nFallbackStatTrak"));
	GET_NETVAR(int, get_fallback_paint_kit, _("DT_BaseAttributableItem"), _("m_nFallbackPaintKit"));
	GET_NETVAR(int, get_fallback_seed, _("DT_BaseAttributableItem"), _("m_nFallbackSeed"));
	GET_NETVAR(float, get_fallback_wear, _("DT_BaseAttributableItem"), _("m_flFallbackWear"));
	GET_NETVAR(bool, is_initialized, _("DT_BaseAttributableItem"), _("m_bInitialized"));
	GET_NETVAR(int, get_entity_level, _("DT_BaseAttributableItem"), _("m_iEntityLevel"));
	GET_NETVAR(int, get_account_id, _("DT_BaseAttributableItem"), _("m_iAccountID"));
	GET_NETVAR(int, get_item_id_low, _("DT_BaseAttributableItem"), _("m_iItemIDLow"));
	GET_NETVAR(int, get_item_id_high, _("DT_BaseAttributableItem"), _("m_iItemIDHigh"));
	GET_NETVAR(int, get_entity_quality, _("DT_BaseAttributableItem"), _("m_iEntityQuality"));
	GET_NETVAR(char, get_custom_name, _("DT_BaseAttributableItem"), _("m_szCustomName"));
	GET_NETVAR(short, get_idx, _("DT_BaseAttributableItem"), _("m_iItemDefinitionIndex"));
};

class c_weapon : public c_base_attributable_item
{
public:
	GET_NETVAR(float, get_last_shot_time, _("DT_WeaponCSBase"), _("m_fLastShotTime"));
	GET_NETVAR(int, get_ammo, _("DT_BaseCombatWeapon"), _("m_iClip1"));
	GET_NETVAR(c_base_handle, get_owner, _("DT_BaseCombatWeapon"), _("m_hOwner"));
	GET_NETVAR(c_base_handle, get_weapon_world_model, _("DT_BaseCombatWeapon"), _("m_hWeaponWorldModel"));
	GET_NETVAR(float, get_next_primary_attack, _("DT_BaseCombatWeapon"), _("m_flNextPrimaryAttack"));
	GET_NETVAR(float, get_next_secondary_attack, _("DT_BaseCombatWeapon"), _("m_flNextSecondaryAttack"));
	GET_NETVAR(int, world_dropped_model_index, _("DT_BaseCombatWeapon"), _("m_iWorldDroppedModelIndex"));
	GET_NETVAR(bool, get_pin_pulled, _("DT_BaseCSGrenade"), _("m_bPinPulled"));
	GET_NETVAR(float, get_throw_time, _("DT_BaseCSGrenade"), _("m_fThrowTime"));
	GET_NETVAR(int, get_zoom_level, _("DT_WeaponCSBaseGun"), _("m_zoomLevel"));
	GET_NETVAR(float, get_post_pone_fire_ready_time, _("DT_WeaponCSBase"), _("m_flPostponeFireReadyTime"));
	GET_NETVAR(float, get_recoil_index, _("DT_WeaponCSBase"), _("m_flRecoilIndex"));
	GET_NETVAR(float, get_accuracy_penalty, _("DT_WeaponCSBase"), _("m_fAccuracyPenalty"));
	GET_NETVAR(float, get_throw_strength, _("DT_BaseCSGrenade"), _("m_flThrowStrength"));
	GET_OFFSET(get_thrower, c_handle < c_base_player >, 0x29B0);
	GET_DATAMAP(int, m_activity);

	std::string get_name();
	int get_weapon_type();

	bool is_smg();
	bool is_grenade();
	bool is_sniper();
	bool is_rifle();
	bool is_bomb();
	bool is_knife();
	bool is_gun();

	void update_accuracy_penalty();
	float get_spread();
	float get_inaccuracy();
	c_weapon_info* get_weapon_data();
};

class c_breakable_surface : public c_base_entity, public i_breakable_with_prop_data
{
public:
	GET_NETVAR(bool, is_broken, _("DT_BreakableSurface"), _("m_bIsBroken"));
};
