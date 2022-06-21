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
	const auto& handle = get_active_weapon_handle();
	if (!handle)
		return nullptr;

	return handle.get();
}

vec3_t c_base_player::get_shoot_pos()
{
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

void c_base_player::invalidate_bone_cache()
{
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

vec3_t c_base_player::get_eye_pos()
{
	if (!this)
		return {};

	return call_vfunc<vec3_t(__thiscall*)(void*)>(this, 277)(this);
}

void c_base_player::invalidate_physics_recursive(const int flag)
{
	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/baseentity_shared.cpp#L1632 */
	static auto invalidate_physics_recursive_fn = reinterpret_cast<void(__thiscall*)(void*, int)>(c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 83 EC 0C 53 8B 5D 08 8B C3")));
	return invalidate_physics_recursive_fn(this, flag);
}

void c_base_player::select_item(const char* string, const int sub_type)
{
	static auto select_item_fn = reinterpret_cast <void(__thiscall*)(void*, const char*, int)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06")));
	return select_item_fn(this, string, sub_type);
}

bool c_base_player::using_standard_weapons_in_vehicle()
{
	static auto sig_fn = reinterpret_cast <bool(__thiscall*)(void*)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41")));
	return sig_fn(this);
}

bool c_base_player::post_think_v_physics()
{
	static auto sig_fn = reinterpret_cast <bool(__thiscall*)(void*)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB")));
	return sig_fn(this);
}

void c_base_player::physics_simulated_entites()
{
	static auto sig_fn = reinterpret_cast <void(__thiscall*)(void*)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74")));
	return sig_fn(this);
}

bool c_base_player::physics_run_think(const int index)
{
	static auto physics_run_think_fn = reinterpret_cast <bool(__thiscall*)(void*, int)> (c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 EC 10 53 56 57 8B F9 8B 87")));
	return physics_run_think_fn(this, index);
}

void c_base_player::post_think()
{
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

animstate_t* c_base_player::get_anim_state()
{
	return *reinterpret_cast<animstate_t**>(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 0x9960));
}

animlayer_t* c_base_player::get_anim_layers() {
	return *reinterpret_cast<animlayer_t**>(reinterpret_cast<DWORD>(this) + 0x2990);
}

int c_base_player::get_sequence_activity(const int sequence)
{
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

c_weapon_info* c_weapon::get_weapon_data() const
{
	return g_sdk.m_interfaces.m_weapon_system->get_wpn_data(this->get_idx());
}

std::string c_weapon::get_name() const
{
	if (!this->get_weapon_data())
		return _("invalid");

	return { this->get_weapon_data()->m_weapon_name };
}

int c_weapon::get_weapon_type() const
{
	if (!this->get_weapon_data())
		return 0;

	return this->get_weapon_data()->m_weapon_type;
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