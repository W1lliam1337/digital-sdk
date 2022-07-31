#include "../hooks.h"

// @note: fixing weird landing animation
// @xref: https://www.unknowncheats.me/forum/counterstrike-global-offensive/504841-fixing-weird-landing-animation.html
void __fastcall hooks::hk_modify_eye_position(void* ecx, void* edx, c_vec3& input_eye_pos)
{
	const auto anim_state = static_cast<c_anim_state*>(ecx);
	if (!anim_state)
		return og::m_modify_eye_position(ecx, edx, input_eye_pos);

	if (!anim_state->m_hit_ground && anim_state->m_duck_amount == 0.0f)
	{
		anim_state->m_smooth_height_valid = false;
		anim_state->m_camera_smooth_height = 0x7F7FFFFF;
		return;
	}

	static auto lookup_bone = reinterpret_cast<int(__thiscall*)(void*, const char*)>(utils::sig(modules::m_client_dll, _("55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20 85 C0 74 07 8B CE E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 0F 84")));
	const auto head_bone = lookup_bone(anim_state->m_player, _("head_0"));
	if (head_bone == -1)
		return;

	auto head_pos = anim_state->m_player->get_hitbox_pos(head_bone);
	head_pos.z += 1.7f;

	if (input_eye_pos.z <= head_pos.z)
		return;

	const auto lol = abs(input_eye_pos.z - head_pos.z);
	float value = (lol - 4.0) * 0.16666667;
	value = std::clamp(value, 0.0f, 1.0f);

	const auto value_squared = value * value;
	input_eye_pos.z = (head_pos.z - input_eye_pos.z) * (value_squared * 3.0 - value_squared * 2.0 * value) + input_eye_pos.z;
}
