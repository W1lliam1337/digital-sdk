#include "../hooks.h"

// @note: fix legs shuffle when your yaw changes
// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/c_cs_player.cpp#L6536
void __fastcall hooks::hk_do_extra_bone_processing( c_base_player* player, void* edx, int a2, int a3, int a4, int a5, int a6, int a7 ) {
	// don't do anything here...
	// @note: or we can change the value of anim_state->m_on_ground to skip the block of code responsible for keeping the toes off the ground.
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/c_cs_player.cpp#L6583
	return;
}

bool __fastcall hooks::hk_should_skip_anim_frame( c_base_player* player, void* edx ) {
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/c_baseanimating.cpp#L3245
	return false;
}

void __fastcall hooks::hk_build_transformations( c_base_player* player, void* edx, void* a2, int a3, int a4, int a5, int a6, int a7 ) {
	if ( !player || !player->is_alive( ) || !player->is_player( ) || player->get_team( ) == ctx::local( )->get_team( ) )
		return og::m_build_transformations( player, edx, a2, a3, a4, a5, a6, a7 );

	// @note: skip the block of code responsible for jiggle bones
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/c_baseanimating.cpp#L1729
	const auto backup_jiggle_bones = player->get_jiggle_bones( );
	player->get_jiggle_bones( ) = false;
	{
		og::m_build_transformations( player, edx, a2, a3, a4, a5, a6, a7 );
	}
	player->get_jiggle_bones( ) = backup_jiggle_bones;
}

void __fastcall hooks::hk_standard_blending_rules( c_base_player* player, void* edx, void* a2, int a3, void* a4, int a5, int a6 ) {
	if ( !player || !player->is_alive( ) || !player->is_player( ) || player->get_team( ) == ctx::local( )->get_team( ) )
		return og::m_standard_blending_rules( player, edx, a2, a3, a4, a5, a6 );

	// @note: disable interpolation
	player->get_effects( ) |= ef_no_interp;
	{
		og::m_standard_blending_rules( player, edx, a2, a3, a4, a5, a6 );
	}
	player->get_effects( ) &= ~ef_no_interp;
}

// @note: we made a 'rebuild' setup of bones by changing all the necessary functions (do_extra_bone_processing, should_skip_anim_frame, build_transformations, standard_blending_rules) for ourselves
// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/c_baseanimating.cpp#L2983
bool __fastcall hooks::hk_setup_bones( c_base_player* player, void* edx, matrix_t* matrix, int max_bones, int bone_mask, float current_time ) {
	const auto pplayer = reinterpret_cast<c_base_player*>(reinterpret_cast<std::uintptr_t>(player) - 0x4);
	if ( !pplayer || !pplayer->is_alive( ) || !pplayer->is_player( ) || pplayer->get_team( ) == ctx::local( )->get_team( ) )
		return og::m_setup_bones( player, edx, matrix, max_bones, bone_mask, current_time );

	og::m_setup_bones( player, edx, matrix, max_bones, bone_mask, current_time );

	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/c_baseanimating.cpp#L3410
	if ( matrix && max_bones >= pplayer->get_bone_cache( ).count( ) )
		memcpy( matrix, pplayer->get_bone_cache( ).base( ), sizeof( matrix_t ) * pplayer->get_bone_cache( ).count( ) );

	return true;
}

void __fastcall hooks::hk_calculate_view( c_base_player* player, void* edx, c_vec3& eye_origin, qangle_t& eye_angles, float& z_near, float& z_far, float& fov ) {
	// @note: fixing weird landing animation
	// @xref: https://www.unknowncheats.me/forum/counterstrike-global-offensive/504841-fixing-weird-landing-animation.html
	if ( !player || !ctx::local( ) || player != ctx::local( ) )
		return og::m_calculate_view( player, edx, eye_origin, eye_angles, z_near, z_far, fov );

	const auto backup_use_new_anim_state = player->should_use_new_anim_state( );

	player->should_use_new_anim_state( ) = false;
	{
		og::m_calculate_view( player, edx, eye_origin, eye_angles, z_near, z_far, fov );
	}
	player->should_use_new_anim_state( ) = backup_use_new_anim_state;
}

// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/client/cstrike15/c_cs_player.cpp#L7318
float __fastcall hooks::hk_get_fov( void* ecx, void* edx ) {
	const auto player = static_cast<c_base_player*>(ecx);
	if ( !player || player != ctx::local( ) )
		return og::m_get_fov( ecx, edx );

	return player->is_scoped( ) ? og::m_get_fov( ecx, edx ) : g_cfg.m_misc.m_fov;
}