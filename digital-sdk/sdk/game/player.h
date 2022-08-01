#pragma once
#include "weapon.h"
#include "breakable_surface.h"

class c_base_player : public c_base_entity
{
public:
	GET_NETVAR(int, get_flags, _("DT_BasePlayer"), _("m_fFlags"));
	GET_NETVAR(int, get_health, _("DT_BasePlayer"), _("m_iHealth"));
	GET_NETVAR(int, get_sequence, _("DT_BasePlayer"), _("m_nSequence"));
	GET_NETVAR(int, get_tick_base, _("DT_BasePlayer"), _("m_nTickBase"));
	GET_NETVAR(int, get_impulse, _("DT_CSPlayer"), _("m_nImpulse"));
	GET_NETVAR(c_handle < c_base_player >, get_vehicle, _("DT_BasePlayer"), _("m_hVehicle"));
	GET_NETVAR(float, get_fall_velocity, _("DT_BasePlayer"), _("m_flFallVelocity"));
	GET_NETVAR(c_vec3, get_origin, _("DT_BaseEntity"), _("m_vecOrigin"));
	GET_NETVAR(int, get_life_state, _("DT_BasePlayer"), _("m_lifeState"));
	GET_NETVAR(bool, has_heavy_armor, _("DT_CSPlayer"), _("m_bHasHeavyArmor"));
	GET_NETVAR(bool, has_helmet, _("DT_CSPlayer"), _("m_bHasHelmet"));
	GET_NETVAR(int, get_armour_value, _("DT_CSPlayer"), _("m_ArmorValue"));
	GET_NETVAR(qangle_t, get_aim_punch_angle, _("DT_BasePlayer"), _("m_aimPunchAngle"));
	GET_NETVAR(c_handle < c_weapon >, get_active_weapon_handle, _("DT_BaseCombatCharacter"), _("m_hActiveWeapon"));
	GET_NETVAR(bool, is_defusing, _("DT_BasePlayer"), _("m_bIsDefusing"));
	GET_NETVAR(bool, is_scoped, _("DT_CSPlayer"), _("m_bIsScoped"));
	GET_NETVAR(c_vec3, get_velocity, _("DT_BasePlayer"), _("m_vecVelocity[0]"));
	GET_NETVAR(bool, get_client_side_animation, _("DT_BaseAnimating"), _("m_bClientSideAnimation"));
	GET_NETVAR(float, get_lby, _("DT_CSPlayer"), _("m_flLowerBodyYawTarget"));
	GET_NETVAR(float, get_duck_amount, _("DT_BasePlayer"), _("m_flDuckAmount"));
	GET_NETVAR(float, get_duck_speed, _("DT_BasePlayer"), _("m_flDuckSpeed"));
	GET_NETVAR(float, get_sim_time, _("DT_CSPlayer"), _("m_flSimulationTime"));

	GET_NETVAR_OFFSET(float, get_old_sim_time, _("DT_CSPlayer"), _("m_flSimulationTime"), 0x4);

	GET_OFFSET(int, get_occlusion_frame, 0xA30);
	GET_OFFSET(int, get_occlusion_mask, 0xA28);
	GET_OFFSET(int, get_client_effects, 0x68);
	GET_OFFSET(int, get_last_skip_framecount, 0xA68);
	GET_OFFSET(int, get_most_recent_model_bone_counter, 2690);
	GET_OFFSET(bool, get_maintain_sequence_transition, 0x9F0);
	GET_OFFSET(bool, get_jiggle_bones, 0x2930);
	GET_OFFSET(uint32_t, get_effects, 0xF0);
	GET_OFFSET(c_vec3, get_abs_velocity, 0x94);
	GET_OFFSET(int, get_eflags, 0xE8);
	GET_OFFSET(int, get_take_damage, 0x280);
	GET_OFFSET(c_bone_accressor, get_bone_accessor, 0x26A8);
	GET_OFFSET(c_utl_vector<matrix_t>, get_bone_cache, 0x2914);
	GET_OFFSET(bool, should_use_new_anim_state, 0x9B14);
	GET_OFFSET(studiohdr_t*, get_model_ptr, 0x2950);
	GET_OFFSET(anim_layer_t*, get_anim_layers, 0x2990);
	GET_OFFSET(int, get_custom_blending_rule_mask, 0xA24);
	GET_OFFSET(float, get_spawn_time, 0x103C0);

	GET_VFUNC(bool(__thiscall*)(void*), is_player(), 158);
	GET_VFUNC(int(__thiscall*)(void*), is_max_health(), 122);
	GET_VFUNC(const char* (__thiscall*)(void*), get_classname(), 59);
	GET_VFUNC(unsigned int(__thiscall*)(void*), physics_solid_mask_for_entity(), 151);
	GET_VFUNC(void(__thiscall*)(void*), get_think(), 139);
	GET_VFUNC(void(__thiscall*)(void*), get_pre_think(), 318);
	GET_VFUNC(void(__thiscall*)(void*), studio_frame_advance(), 220);
	GET_VFUNC(void(__thiscall*)(void*), update_collistion_bounds(), 340);
	GET_VFUNC(void(__thiscall*)(void*), update_client_side_animations(), 224);

	void set_abs_angles(const c_vec3 angles)
	{
		static const auto set_angles_fn = reinterpret_cast<void(__thiscall*)(void*, const c_vec3&)>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8")));
		set_angles_fn(this, angles);
	}

	void set_abs_origin(const c_vec3 position)
	{
		static const auto set_pos_fn = reinterpret_cast<void(__thiscall*)(void*, const c_vec3&)>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8")));
		set_pos_fn(this, position);
	}

	int sequence_activity(const int sequence)
	{
		const auto hdr = interfaces::m_model_info->get_studio_model(this->get_model());

		if (!hdr)
			return 0;

		static const auto sequence_activity_fn = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(utils::sig(modules::m_client_dll, _("55 8B EC 53 8B 5D 08 56 8B F1 83")));
		return sequence_activity_fn(this, hdr, sequence);
	}

	void setup_bones_attachment_helper()
	{
		static const auto sig_fn = reinterpret_cast<void(__thiscall*)(void*, void*)>(utils::sig(modules::m_client_dll, _("55 8B EC 83 EC 48 53 8B 5D 08 89 4D F4")));
		return sig_fn(this, this->get_model_ptr());
	}

	void invalidate_bone_cache()
	{
		static const auto invalidate_bone_cache_fn = reinterpret_cast<std::uintptr_t>(utils::sig(modules::m_client_dll,
			_("80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81")) + 10);

		*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x2924) = 0xFF7FFFFF;
		*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x2690) = **reinterpret_cast<std::uintptr_t**>(invalidate_bone_cache_fn) - 1;
	}

	bool is_armored(const int hit_group) const
	{
		bool is_armored = false;

		if (this->get_armour_value() > 0)
		{
			switch (hit_group)
			{
			case hitgroup_generic:
			case hitgroup_chest:
			case hitgroup_stomach:
			case hitgroup_leftarm:
			case hitgroup_rightarm:
			case hitgroup_neck:
				is_armored = true;
				break;
			case hitgroup_head:
				if (this->has_helmet())
					is_armored = true;
			case hitgroup_leftleg:
			case hitgroup_rightleg:
				if (this->has_heavy_armor())
					is_armored = true;
				break;
			default: break;
			}
		}

		return is_armored;
	}

	void set_sequence(const int flag)
	{
		return utils::call_vfunc<void(__thiscall*)(void*, int)>(this, 219)(this, flag);
	}

	c_vec3 get_hitbox_pos(const int hitbox_id)
	{
		const auto studio_model = interfaces::m_model_info->get_studio_model(this->get_model());
		if (studio_model)
		{
			const auto hitbox = studio_model->get_hitbox_set(hitbox_head)->get_hitbox(hitbox_id);
			if (hitbox)
			{
				c_vec3 min, max;

				math::vector_transform(hitbox->m_mins, get_bone_cache()[hitbox->m_bone], min);
				math::vector_transform(hitbox->m_maxs, get_bone_cache()[hitbox->m_bone], max);

				return (min + max) / 2.0f;
			}
		}

		return c_vec3{};
	}

	void invalidate_physics_recursive(const int flag)
	{
		// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/baseentity_shared.cpp#L1632
		static const auto invalidate_physics_recursive_fn = reinterpret_cast<void(__thiscall*)(void*, int)>(utils::sig(
			modules::m_client_dll, _("55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3")));
		return invalidate_physics_recursive_fn(this, flag);
	}

	void select_item(const char* string, const int sub_type)
	{
		static const auto select_item_fn = reinterpret_cast<void(__thiscall*)(void*, const char*, int)>(utils::sig(
			modules::m_client_dll, _("55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06")));
		return select_item_fn(this, string, sub_type);
	}

	bool using_standard_weapons_in_vehicle()
	{
		static const auto sig_fn = reinterpret_cast<bool(__thiscall*)(void*)>(utils::sig(
			modules::m_client_dll, _("56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41")));
		return sig_fn(this);
	}

	bool post_think_v_physics()
	{
		static const auto sig_fn = reinterpret_cast<bool(__thiscall*)(void*)>(utils::sig(
			modules::m_client_dll, _("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB")));
		return sig_fn(this);
	}

	void physics_simulated_entites()
	{
		static const auto sig_fn = reinterpret_cast<void(__thiscall*)(void*)>(utils::sig(
			modules::m_client_dll, _("56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74")));
		return sig_fn(this);
	}

	bool physics_run_think(const int index)
	{
		static const auto physics_run_think_fn = reinterpret_cast<bool(__thiscall*)(void*, int)>(utils::sig(
			modules::m_client_dll, _("55 8B EC 83 EC 10 53 56 57 8B F9 8B 87")));
		return physics_run_think_fn(this, index);
	}

	void post_think()
	{
		interfaces::m_mdl_cache->begin_lock();
		{
			if (this->is_alive())
			{
				this->update_collistion_bounds();

				if (this->get_flags() & fl_on_ground)
					this->get_fall_velocity() = 0.f;

				if (this->get_sequence() == -1)
					this->set_sequence(0);

				this->studio_frame_advance();
				this->post_think_v_physics();
			}

			this->physics_simulated_entites();
		}
		interfaces::m_mdl_cache->end_lock();
	}

	c_vec3 get_shoot_pos()
	{
		// @xref: https://www.unknowncheats.me/forum/counterstrike-global-offensive/338731-weapon_shootpos-rebuilt-server-code.html
		// @xref: https://www.unknowncheats.me/forum/counterstrike-global-offensive/277792-getting-eye-position-m_vecviewoffset.html

		static const auto eye_pos_fn = reinterpret_cast<float* (__thiscall*)(void*, c_vec3*)>(utils::sig(
			modules::m_client_dll, _("55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?"))); // 8B 07 FF 90 ? ? ? ? 80 BF ? ? ? ? ? 74 10 
		c_vec3 eye_pos;
		eye_pos_fn(this, &eye_pos);
		return eye_pos;
	}

	[[nodiscard]] c_weapon* get_active_weapon() const
	{
		const auto& handle = get_active_weapon_handle();
		if (!handle)
			return nullptr;

		return handle.get();
	}

	[[nodiscard]] bool is_alive() const
	{
		return this->get_health() > 0 && this->get_life_state() == life_alive;
	}

	bool is_breakable()
	{
		// @ida: https://imgur.com/D00fKg4
		// module: server.dll; sig: (+0x54) E8 ? ? ? ? 84 C0 75 A1
		// module: client.dll; sig: 55 8B EC 51 56 8B F1 85 F6 74 68

		// @ida: https://prnt.sc/fpPs3Y6VwsYP
		static const auto is_breakable = utils::sig(modules::m_client_dll, _("55 8B EC 51 56 8B F1 85 F6 74 68"));

		const auto take_damage = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(is_breakable) + 0x26);
		const auto backup = *reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + take_damage);

		const auto client_class = this->get_client_class();
		const auto network_name = client_class->m_network_name;

		if (!strcmp(network_name, "CBreakableSurface"))
			*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + take_damage) = 2;
		else if (!strcmp(network_name, "CBaseDoor") || !strcmp(network_name, "CDynamicProp"))
			*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + take_damage) = 0;

		const auto result = reinterpret_cast<bool(__thiscall*)(void*)>(is_breakable)(this);
		*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + take_damage) = backup;
		return result;
	}

	c_anim_state* get_anim_state()
	{
		return *static_cast<c_anim_state**>(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 0x9960));
	}
};