#include "../hooks.h"

void __fastcall hooks::hk_get_color_modulation( i_material* material, void* edx, float* r, float* g, float* b ) {
	if ( !ctx::local( ) || !interfaces::m_engine->is_connected( ) || !interfaces::m_engine->is_in_game( ) )
		return og::m_get_color_modulation( material, edx, r, g, b );

	og::m_get_color_modulation( material, edx, r, g, b );

	if ( !g_cfg.m_visuals.m_world_modulation )
		return og::m_get_color_modulation( material, edx, r, g, b );

	if ( !material || material->is_error_material( ) )
		return og::m_get_color_modulation( material, edx, r, g, b );

	const auto group = material->get_texture_group_name( );

	if ( strstr( group, _( "World textures" ) )
		 || strstr( group, _( "StaticProp textures" ) )
		 || strstr( group, _( "SkyBox textures" ) ) ) {
		const auto is_prop = group == _( "StaticProp textures" );

		*r *= is_prop ? 0.5f * (g_cfg.m_visuals.m_world_modulation_color[0] / 255.0f) : 0.23f * (g_cfg.m_visuals.m_world_modulation_color[0] / 255.0f);
		*g *= is_prop ? 0.5f * (g_cfg.m_visuals.m_world_modulation_color[1] / 255.0f) : 0.23f * (g_cfg.m_visuals.m_world_modulation_color[1] / 255.0f);
		*b *= is_prop ? 0.5f * (g_cfg.m_visuals.m_world_modulation_color[2] / 255.0f) : 0.23f * (g_cfg.m_visuals.m_world_modulation_color[2] / 255.0f);
	}
}

float __fastcall hooks::hk_get_alpha_modulation( i_material* material, void* edx ) {
	if ( !ctx::local( ) || !interfaces::m_engine->is_connected( ) || !interfaces::m_engine->is_in_game( ) )
		return og::m_get_alpha_modulation( material, edx );

	if ( !g_cfg.m_visuals.m_world_modulation )
		return og::m_get_alpha_modulation( material, edx );

	if ( !material || material->is_error_material( ) )
		return og::m_get_alpha_modulation( material, edx );

	const auto group = material->get_texture_group_name( );
	if ( strstr( group, _( "World textures" ) )
		 || strstr( group, _( "StaticProp textures" ) )
		 || strstr( group, _( "SkyBox textures" ) ) )
		return g_cfg.m_visuals.m_world_alpha / 255.0f;

	return og::m_get_alpha_modulation( material, edx );
}