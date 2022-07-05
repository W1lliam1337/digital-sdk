#pragma once
#include "../../sdk/sdk.h"
#include "../../includes.h"

class c_chams
{
protected:
	//i_material* m_material_texture{};
public:
	bool draw(void* ecx, void* edx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bones);
};

inline c_chams g_chams;