#include "entity.h"
#include "../../dependencies/utils.h"

bool c_base_player::is_alive()
{
	if (!this)
		return false;

	return this->get_health() > 0 && this->get_life_state() == life_alive;
}

c_weapon* c_base_player::get_active_weapon() const
{
	if (!this)
		return nullptr;

	const auto& handle = get_active_weapon_handle();
	if (!handle)
		return nullptr;

	return handle.get();
}

vec3_t c_base_player::get_shoot_pos()
{
	if (!this)
		return {};

	/* @uc: https://www.unknowncheats.me/forum/counterstrike-global-offensive/338731-weapon_shootpos-rebuilt-server-code.html
	 * https://www.unknowncheats.me/forum/counterstrike-global-offensive/277792-getting-eye-position-m_vecviewoffset.html
	 * @ida:
		void __thiscall sub_10406810(struct_anim_state **this, struct_s *a2)
		{
			  struct_astate *v3; // ecx

			  (*&(*this)->gap10A[410])(this, a2);
			  if ( *(this + 0x9B14) )
			  {
			    v3 = this[0x2658];
			    if ( v3 )
			      modifyeyepos(v3, a2);
			  }
		}
	*/

	static auto eye_pos_fn = reinterpret_cast<float* (__thiscall*)(void*, vec3_t*)>(c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90 ? ? ? ?"))); // 8B 07 FF 90 ? ? ? ? 80 BF ? ? ? ? ? 74 10 
	vec3_t eye_pos;
	eye_pos_fn(this, &eye_pos);
	return eye_pos;
}

vec3_t c_base_player::get_hitbox_pos(const int hitbox_id)
{
	if (!this)
		return {};

	matrix_t bone_matrix[MAXSTUDIOBONES];

	if (setup_bones(bone_matrix, MAXSTUDIOBONES, bone_used_by_hitbox, 0.0f)) 
	{
		const auto studio_model = g_sdk.m_interfaces.m_model_info->get_studio_model(this->get_model());
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

void c_base_player::invalidate_bone_cache()
{
	if (!this)
		return;

	/* @ida:
	 * int __thiscall sub_101CF430(_DWORD *this)
		{
		  int result; // eax

		  if ( byte_10D740E0 )
		  {
			result = dword_152F1D64 - 1;
			this[0xA4A] = 0xFF7FFFFF;
			this[0x9A4] = result;
		  }
		  return result;
		}
	*/

	static auto p_model_bone_counter = static_cast<void*>(c_utils::find_sig(
		g_sdk.m_modules.m_client_dll, _("80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81")));
	const int n_model_bone_counter = **reinterpret_cast<int**>(reinterpret_cast<DWORD>(p_model_bone_counter) + 10);

	*reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + 0x2928) = -FLT_MAX;
	*reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x2690) = n_model_bone_counter - 1;
}

vec3_t& c_base_player::get_abs_origin()
{
	return call_vfunc<vec3_t& (__thiscall*)(void*)>(this, 10)(this);
}

bool c_base_player::is_weapon()
{
	if (!this)
		return false;

	return call_vfunc<bool(__thiscall*)(void*)>(this, 166)(this);
}

bool c_base_player::is_player()
{
	if (!this)
		return false;

	return call_vfunc<bool(__thiscall*)(void*)>(this, 158)(this);
}

unsigned int c_base_player::physics_solid_mask_for_entity()
{
	if (!this)
		return 0;

	return call_vfunc<unsigned int(__thiscall*)(void*)>(this, 151)(this);
}

void c_base_player::get_think()
{
	if (!this)
		return;

	return call_vfunc<void(__thiscall*)(void*)>(this, 139)(this);
}

void c_base_player::get_pre_think()
{
	if (!this)
		return;

	return call_vfunc<void(__thiscall*)(void*)>(this, 318)(this);
}

void c_base_player::set_sequence(const int flag)
{
	if (!this)
		return;

	return call_vfunc<void(__thiscall*)(void*, int)>(this, 219)(this, flag);
}

void c_base_player::studio_frame_advance()
{
	if (!this)
		return;

	return call_vfunc<void(__thiscall*)(void*)>(this, 220)(this);
}

void c_base_player::update_collistion_bounds()
{
	if (!this)
		return;

	return call_vfunc<void(__thiscall*)(void*)>(this, 340)(this);
}

void c_base_player::update_client_side_animations()
{
	if (!this)
		return;

	return call_vfunc<void(__thiscall*)(void*)>(this, 224)(this);
}
 
vec3_t& c_base_player::get_eye_pos()
{
	return call_vfunc<vec3_t&(__thiscall*)(void*)>(this, 168)(this);
}

int c_base_player::is_max_health()
{
	if (!this)
		return 0;

	return call_vfunc<int(__thiscall*)(void*)>(this, 122)(this);
}

const char* c_base_player::get_classname()
{
	if (!this)
		return {};

	return call_vfunc<const char*(__thiscall*)(void*)>(this, 59)(this);
}

void c_base_player::invalidate_physics_recursive(const int flag)
{
	if (!this)
		return;

	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/baseentity_shared.cpp#L1632 */
	static auto invalidate_physics_recursive_fn = reinterpret_cast<void(__thiscall*)(void*, int)>(c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3")));
	return invalidate_physics_recursive_fn(this, flag);
}

void c_base_player::select_item(const char* string, const int sub_type)
{
	if (!this)
		return;

	static auto select_item_fn = reinterpret_cast <void(__thiscall*)(void*, const char*, int)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06")));
	return select_item_fn(this, string, sub_type);
}

bool c_base_player::using_standard_weapons_in_vehicle()
{
	if (!this)
		return false;

	static auto sig_fn = reinterpret_cast <bool(__thiscall*)(void*)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41")));
	return sig_fn(this);
}

bool c_base_player::post_think_v_physics()
{
	if (!this)
		return false;

	static auto sig_fn = reinterpret_cast <bool(__thiscall*)(void*)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB")));
	return sig_fn(this);
}

void c_base_player::physics_simulated_entites()
{
	if (!this)
		return;

	static auto sig_fn = reinterpret_cast <void(__thiscall*)(void*)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74")));
	return sig_fn(this);
}

bool c_base_player::physics_run_think(const int index)
{
	if (!this)
		return false;

	static auto physics_run_think_fn = reinterpret_cast <bool(__thiscall*)(void*, int)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 EC 10 53 56 57 8B F9 8B 87")));
	return physics_run_think_fn(this, index);
}

void c_base_player::post_think()
{
	if (!this)
		return;

	g_sdk.m_interfaces.m_model_cache->begin_lock();
	{
		if (this->is_alive())
		{
			this->update_collistion_bounds();

			if (this->get_flags().has(fl_on_ground))
				this->get_fall_velocity() = 0.f;

			if (this->get_sequence() == -1)
				this->set_sequence(0);

			this->studio_frame_advance();
			this->post_think_v_physics();
		}

		this->physics_simulated_entites();
	}
	g_sdk.m_interfaces.m_model_cache->end_lock();
}

bool c_base_player::is_breakable()
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
		else if (physics_solid_mask_for_entity() & CONTENTS_PLAYERCLIP)
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

c_animstate* c_base_player::get_anim_state()
{
	if (!this)
		return nullptr;

	/* @ida: https://prnt.sc/26pb622 */
	return *reinterpret_cast<c_animstate**>(reinterpret_cast<DWORD>(this) + 0x9960);
}

animlayer_t* c_base_player::get_anim_layers()
{
	if (!this)
		return nullptr;

	return *reinterpret_cast<animlayer_t**>(reinterpret_cast<DWORD>(this) + 0x2990);
}

void c_base_player::set_abs_origin(vec3_t position) {

	static auto set_position_fn = reinterpret_cast<void(__thiscall*)(void*, const vec3_t&)>(c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8")));
	set_position_fn(this, position);
}

int c_base_player::get_sequence_activity(const int sequence)
{
	if (!this)
		return 0;

	studiohdr_t* studio_hdr = g_sdk.m_interfaces.m_model_info->get_studio_model(this->get_model());
	if (!studio_hdr)
		return -1;

	static auto sequence_activity_fn = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 53 8B 5D 08 56 8B F1 83")));
	return sequence_activity_fn(this, studio_hdr, sequence);
}

void c_weapon::update_accuracy_penalty()
{
	if (!this)
		return;

	call_vfunc<void(__thiscall*)(void*)>(this, 484)(this);
}

float c_weapon::get_spread()
{
	if (!this)
		return 0.0f;

	return call_vfunc<float(__thiscall*)(void*)>(this, 453)(this);
}

float c_weapon::get_inaccuracy()
{
	if (!this)
		return 0.0f;

	return call_vfunc<float(__thiscall*)(void*)>(this, 483)(this);
}

c_weapon_info* c_weapon::get_weapon_data()
{
	/* (+0x7) E9 ? ? ? ? 8B 4B 24 */
	if (!this)
		return nullptr;

	return call_vfunc<c_weapon_info*(__thiscall*)(void*)>(this, 461)(this);
	//return g_sdk.m_interfaces.m_weapon_system->get_weapon_data(get_idx());
}

std::string c_weapon::get_name()
{
	if (this->get_weapon_data())
		return _("invalid");

	return { this->get_weapon_data()->m_weapon_name };
}

int c_weapon::get_weapon_type()
{
	if (!this->get_weapon_data())
		return 0;

	return get_weapon_data()->m_weapon_type;
}

bool c_weapon::is_smg()
{
	if (!this)
		return false;

	return get_weapon_type() == weapon_type_submachinegun;
}

bool c_weapon::is_grenade()
{
	if (!this)
		return false;

	return get_weapon_type() == weapon_type_grenade;
}

bool c_weapon::is_gun()
{
	if (!this)
		return false;

	switch (get_weapon_type())
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

bool c_weapon::is_knife()
{
	if (!this)
		return false;

	return get_weapon_type() == weapon_type_knife;
}

bool c_weapon::is_rifle()
{
	if (!this)
		return false;

	return get_weapon_type() == weapon_type_rifle;
}

bool c_weapon::is_sniper()
{
	if (!this)
		return false;

	return get_weapon_type() == weapon_type_sniper_rifle;
}

bool c_weapon::is_bomb()
{
	if (!this)
		return false;

	return get_weapon_type() == weapon_type_c4;
}