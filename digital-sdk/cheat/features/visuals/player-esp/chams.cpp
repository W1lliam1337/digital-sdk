#include "chams.h"
#include "../../../hooks/hooks.h"

void chams::init_materials()
{
	m_regular = create_material(_("digital_regular.vmt"), _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
			"$basetexture" "vgui/white_additive"
			"$ignorez"      "0"
			"$model"		"1"
			"$flat"			"0"
			"$nocull"		"1"
			"$halflambert"	"1"
			"$nofog"		"1"
			"$wireframe"	"0"
		})#"));
}

i_material* chams::create_material(std::string_view material_name, std::string_view shader_type, std::string_view material_data)
{
	const auto key_values = new c_key_values(shader_type.data());
	key_values->load_from_buffer(material_name.data(), material_data.data());

	return interfaces::m_material_system->create_material(material_name.data(), key_values);
}

void chams::override_material(const c_color& clr, const bool ignorez)
{
	const auto material = m_regular;
	if (!material)
		return;

	material->set_material_var_flag(material_var_ignorez, ignorez ? true : false);

	material->alpha_modulate(clr.a() / 255.f);
	material->color_modulate(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);
	material->increment_reference_count();

	if (const auto $envmaptint = material->find_var(_("$envmaptint"), nullptr, false))
	{
		$envmaptint->set_vector(c_vec3(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f));
	}

	interfaces::m_model_render->force_material(material);
}

bool chams::draw_model(void* ecx, void* edx, void* context, draw_model_state_t& state, model_render_info_t& info, matrix_t* bones)
{
	const auto entity = reinterpret_cast<c_base_entity*>(interfaces::m_entity_list->get_client_entity(info.m_entity_index));
	if (!entity)
		return true;

	const auto player = reinterpret_cast<c_base_player*>(entity);
	if (!player)
		return true;

	// @note: get the player model
	const bool is_player = strstr(info.m_model->m_name, _("models/player"));

	// @note: works only for the enemy, remake it for yourself
	if (is_player && player->is_player() && player != ctx::local() && player->get_team() != ctx::local()->get_team())
	{
		if (g_cfg.m_esp.m_player[1].m_chams)
		{
			// @note: here your stuff override
			if (g_cfg.m_esp.m_player[1].m_invisible_chams)
			{
				override_material(c_color(154, 205, 50), true);
				hooks::og::m_draw_model_execute(ecx, edx, context, state, info, bones);
			}

			override_material(c_color(255, 215, 0), false);
		}

		return true;
	}
	else
	{
		// @note: call the original so as not to break the game (models)
		return true;
	}

	return false; // clear override 
}