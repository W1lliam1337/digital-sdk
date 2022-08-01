#pragma once
#include "../../../../includes.h"
#include "../../../../sdk/sdk.h"

namespace chams
{
	inline i_material* m_regular{};
	void init_materials();
	i_material* create_material(std::string_view material_name, std::string_view shader_type, std::string_view material_data);
	void override_material(const c_color& clr, const bool ignorez);
	bool draw_model(void* ecx, void* edx, void* context, draw_model_state_t& state, model_render_info_t& info, matrix_t* bones);
}