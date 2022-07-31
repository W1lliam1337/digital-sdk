#pragma once
#include "classes.h"
#include "../interfaces/classes/i_client_entity.h"
#include "../../utils/netvar-manager/netvar_manager.h"
#include "../../utils/utils.h"
#include "enums.h"
#include "entity.h"

class c_base_entity;

#define GET_VFUNC( type, function_name, index, ...) \
auto function_name { \
return utils::call_vfunc< type >( this, index )( this, __VA_ARGS__ ); \
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
	GET_NETVAR(int, get_clip1, _("DT_BaseCombatWeapon"), _("m_iClip1"));
	GET_NETVAR(int, get_clip2, _("DT_BaseCombatWeapon"), _("m_iClip2"));
	GET_NETVAR(float, get_postpone_fire_ready_time, _("DT_WeaponCSBase"), _("m_flPostponeFireReadyTime"));
	GET_OFFSET(c_handle < c_base_player >, get_thrower, 0x29B0);
	GET_VFUNC(void(__thiscall*)(void*), update_accuracy_penalty(), 484);
	GET_VFUNC(float(__thiscall*)(void*), get_spread(), 453);
	GET_VFUNC(float(__thiscall*)(void*), get_inaccuracy(), 483);
	GET_VFUNC(c_weapon_info*(__thiscall*)(void*), get_weapon_data(), 461);

	bool is_smg()
	{
		return this->get_weapon_data()->m_weapon_type == weapon_type_submachinegun;
	}

	bool is_grenade()
	{
		return this->get_weapon_data()->m_weapon_type == weapon_type_grenade;
	}

	bool is_gun()
	{
		switch (this->get_weapon_data()->m_weapon_type)
		{
		case weapon_type_grenade:
			return false;
		case weapon_type_knife:
			return false;
		case weapon_c4:
			return false;
		default:
			return true;
		}
	}

	bool is_knife()
	{
		return this->get_weapon_data()->m_weapon_type == weapon_type_knife;
	}

	bool is_rifle()
	{
		return this->get_weapon_data()->m_weapon_type == weapon_type_rifle;
	}

	bool is_sniper()
	{
		return this->get_weapon_data()->m_weapon_type == weapon_type_sniper_rifle;
	}

	bool is_bomb()
	{
		return this->get_weapon_data()->m_weapon_type == weapon_type_c4;
	}
};