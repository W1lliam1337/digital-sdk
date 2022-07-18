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
	GET_NETVAR(vec3_t, get_origin, _("DT_BaseEntity"), _("m_vecOrigin"));
	GET_NETVAR(int, get_life_state, _("DT_BasePlayer"), _("m_lifeState"));
	GET_NETVAR(bool, has_heavy_armor, _("DT_CSPlayer"), _("m_bHasHeavyArmor"));
	GET_NETVAR(bool, has_helmet, _("DT_CSPlayer"), _("m_bHasHelmet"));
	GET_NETVAR(int, get_armour_value, _("DT_CSPlayer"), _("m_ArmorValue"));
	GET_NETVAR(qangle_t, get_aim_punch_angle, _("DT_BasePlayer"), _("m_aimPunchAngle"));
	GET_NETVAR(c_handle < c_weapon >, get_active_weapon_handle, _("DT_BaseCombatCharacter"), _("m_hActiveWeapon"));
	GET_NETVAR(bool, is_defusing, _("DT_BasePlayer"), _("m_bIsDefusing"));
	GET_OFFSET(int, get_take_damage, 0x280);
	GET_OFFSET(c_bone_accressor, get_bone_accessor, 0x26A8);
	GET_OFFSET(CUtlVector<matrix_t>, get_bone_cache, 0x2914);
	GET_OFFSET(bool, should_use_new_anim_state, 0x9B14);
	GET_VFUNC(bool(__thiscall*)(void*), is_player(), 158);
	GET_VFUNC(int(__thiscall*)(void*), is_max_health(), 122);
	GET_VFUNC(const char* (__thiscall*)(void*), get_classname(), 59);
	GET_VFUNC(unsigned int(__thiscall*)(void*), physics_solid_mask_for_entity(), 151);
	GET_VFUNC(void(__thiscall*)(void*), get_think(), 139);
	GET_VFUNC(void(__thiscall*)(void*), get_pre_think(), 318);
	GET_VFUNC(void(__thiscall*)(void*), studio_frame_advance(), 220);
	GET_VFUNC(void(__thiscall*)(void*), update_collistion_bounds(), 340);
	GET_VFUNC(void(__thiscall*)(void*), update_client_side_animations(), 224);

	void set_sequence(const int flag)
	{
		return g_utils->call_vfunc<void(__thiscall*)(void*, int)>(this, 219)(this, flag);
	}

	vec3_t get_hitbox_pos(const int hitbox_id)
	{
		if (!this)
			return {};

		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, bone_used_by_hitbox, 0.0f))
		{
			const auto studio_model = g_interfaces->m_model_info->get_studio_model(this->get_model());
			if (studio_model)
			{
				const auto hitbox = studio_model->get_hitbox_set(hitbox_head)->get_hitbox(hitbox_id);
				if (hitbox)
				{
					vec3_t min, max;

					c_math::vector_transform(hitbox->m_mins, bone_matrix[hitbox->m_bone], min);
					c_math::vector_transform(hitbox->m_maxs, bone_matrix[hitbox->m_bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
		return vec3_t{};
	}

	void invalidate_physics_recursive(const int flag)
	{
		/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/baseentity_shared.cpp#L1632 */
		static auto invalidate_physics_recursive_fn = reinterpret_cast<void(__thiscall*)(void*, int)>(g_utils->find_sig(g_modules->m_client_dll, _("55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3")));
		return invalidate_physics_recursive_fn(this, flag);
	}

	void select_item(const char* string, const int sub_type)
	{
		static auto select_item_fn = reinterpret_cast <void(__thiscall*)(void*, const char*, int)> (g_utils->find_sig(g_modules->m_client_dll, _("55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06")));
		return select_item_fn(this, string, sub_type);
	}

	bool using_standard_weapons_in_vehicle()
	{
		static auto sig_fn = reinterpret_cast <bool(__thiscall*)(void*)> (g_utils->find_sig(g_modules->m_client_dll, _("56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41")));
		return sig_fn(this);
	}

	bool post_think_v_physics()
	{
		static auto sig_fn = reinterpret_cast <bool(__thiscall*)(void*)> (g_utils->find_sig(g_modules->m_client_dll, _("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB")));
		return sig_fn(this);
	}

	void physics_simulated_entites()
	{
		static auto sig_fn = reinterpret_cast <void(__thiscall*)(void*)> (g_utils->find_sig(g_modules->m_client_dll, _("56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74")));
		return sig_fn(this);
	}

	bool physics_run_think(const int index)
	{
		static auto physics_run_think_fn = reinterpret_cast <bool(__thiscall*)(void*, int)> (g_utils->find_sig(g_modules->m_client_dll, _("55 8B EC 83 EC 10 53 56 57 8B F9 8B 87")));
		return physics_run_think_fn(this, index);
	}

	void post_think()
	{
		g_interfaces->m_mdl_cache->begin_lock();
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
		g_interfaces->m_mdl_cache->end_lock();
	}

	vec3_t get_shoot_pos()
	{
		/* @uc: https://www.unknowncheats.me/forum/counterstrike-global-offensive/338731-weapon_shootpos-rebuilt-server-code.html
		 * https://www.unknowncheats.me/forum/counterstrike-global-offensive/277792-getting-eye-position-m_vecviewoffset.html
		*/

		static auto eye_pos_fn = reinterpret_cast<float* (__thiscall*)(void*, vec3_t*)>(g_utils->find_sig(g_modules->m_client_dll, _("55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?"))); // 8B 07 FF 90 ? ? ? ? 80 BF ? ? ? ? ? 74 10 
		vec3_t eye_pos;
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
		/* @ida: https://imgur.com/D00fKg4
		 * module: server.dll; sig: (+0x54) E8 ? ? ? ? 84 C0 75 A1
		 * module: client.dll; sig: 55 8B EC 51 56 8B F1 85 F6 74 68
		*/

		if (!this)
			return false;

		if (this->get_health() < 0 && this->is_max_health() > 0)
			return true;

		if (this->get_take_damage() != 2)
		{
			if (this->get_client_class()->m_class_id != class_id_func_brush)
				return false;
		}

		if (this->get_collision_group() != collision_group_pushaway && this->get_collision_group() != collision_group_breakable_glass
			&& this->get_collision_group() != collision_group_none)
			return false;

		if (this->get_health() > 200)
			return false;

		const auto physics_interface = dynamic_cast<i_multiplayer_physics*>(this);

		if (physics_interface != nullptr)
		{
			if (physics_interface->get_multiplayer_physics_mode() != 1)
				return false;
		}
		else
		{
			const char* class_name = this->get_classname();
			if (!strcmp(class_name, _("func_breakable")) || !strcmp(class_name, _("func_breakable_surf")))
			{
				if (!strcmp(class_name, _("func_breakable_surf")))
				{
					const auto surface = reinterpret_cast<c_breakable_surface*>(this);

					if (surface->is_broken())
						return false;
				}
			}
			else if (physics_solid_mask_for_entity() & contents_playerclip)
			{
				return false;
			}
		}

		const auto breakable_interface = dynamic_cast<i_breakable_with_prop_data*>(this);
		if (breakable_interface)
		{
			if (breakable_interface->get_dmg_mod_bullet() <= 0.0f)
				return false;
		}

		return true;
	}
};