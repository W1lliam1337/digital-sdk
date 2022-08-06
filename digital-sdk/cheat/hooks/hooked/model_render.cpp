#include "../hooks.h"

void __fastcall hooks::hk_draw_model_execute( i_model_render* model_render, void* edx, void* ctx, draw_model_state_t& state, model_render_info_t& info, matrix_t* custom_bone_to_world ) {
	if ( !ctx::local( ) || interfaces::m_model_render->is_forced_material( ) )
		return og::m_draw_model_execute( model_render, edx, ctx, state, info, custom_bone_to_world );

	if ( chams::draw_model( model_render, edx, ctx, state, info, custom_bone_to_world ) )
		og::m_draw_model_execute( model_render, edx, ctx, state, info, custom_bone_to_world );

	interfaces::m_model_render->force_material( nullptr );
}