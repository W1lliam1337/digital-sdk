#include "menu.h"
#include <cmath>

bool menu::init_style() noexcept
{
	auto& io{ ImGui::GetIO() };
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::StyleColorsDark();

	auto& style{ ImGui::GetStyle() };
	style.ScrollbarSize = 13.0f;
	style.WindowTitleAlign = ImVec2{ 0.5f, 0.5f };
	style.WindowRounding = 6.f;

	return true;
}

void menu::color_edit(const char* label, c_color* color)
{
	float a_color[4] =
	{
		color->r() / 255.0f,
		color->g() / 255.0f,
		color->b() / 255.0f,
		color->a() / 255.0f
	};

	if (ImGui::ColorEdit4(label, a_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_RGB))
		color->set_color(a_color[0], a_color[1], a_color[2], a_color[3]);
}

void menu::render()
{
	auto& style = ImGui::GetStyle();
	const auto& io = ImGui::GetIO();

	static float alpha = 0.f;

	const auto& draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(ImVec2{ 0.f, 0.f }, io.DisplaySize, ImColor{ 0.f, 0.f, 0.f, alpha });

	if (!ctx::menu_data::m_is_menu_opened)
	{
		alpha = std::fmax(alpha - io.DeltaTime * 3.0f, 0.f);
		style.Alpha = std::fmax(style.Alpha - io.DeltaTime * 6.0f, 0.f);
		return;
	}

	alpha = std::fmin(alpha + io.DeltaTime * 3.0f, 0.5f);
	style.Alpha = std::fmin(style.Alpha + io.DeltaTime * 6.0f, 1.f);

	static std::uint32_t selected_tab = 0;
	ImGui::SetNextWindowSize(ImVec2{ 512.f, 515.f }, ImGuiCond_Once);
	ImGui::Begin(_("digital csgo sdk"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::BeginChild(_("Tabs Panel"), ImVec2{ 150, 0 }, true);
	{
		static constexpr const char* tabs[]{
			"Rage",
			"Legit",
			"Anti-aim",
			"World esp",
			"Player esp",
			"Misc"
		};
		for (std::size_t i = 0; i < IM_ARRAYSIZE(tabs); ++i)
		{
			if (ImGui::Selectable(tabs[i], selected_tab == i))
			{
				selected_tab = i;
			}
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginGroup();
	{
		switch (selected_tab)
		{
		case e_menu::rage_tab:
			rage_tab();
			break;
		case e_menu::legit_tab:
			legit_tab();
			break;
		case aa_tab:
			antiaim_tab();
			break;
		case e_menu::world_esp_tab:
			world_esp_tab();
			break;
		case e_menu::player_esp_tab:
			player_esp_tab();
			break;
		case e_menu::misc_tab:
			misc_tab();
			break;
		default: break;
		}
	}
	ImGui::EndGroup();
	ImGui::End();
}

void menu::rage_tab()
{
	ImGui::Text("Rage settings");
	ImGui::Separator();
}

void menu::legit_tab()
{
	ImGui::Text("Legit settings");
	ImGui::Separator();

	ImGui::Checkbox(_("Enable legitbot"), &g_cfg.m_legit.m_enabled);
	ImGui::Checkbox(_("Auto fire"), &g_cfg.m_legit.m_auto_fire);
	ImGui::Checkbox(_("Silent aim"), &g_cfg.m_legit.m_silent);

	ImGui::SliderInt(_("Smooth"), &g_cfg.m_legit.m_smooth, 1, 100);
	ImGui::SliderInt(_("FOV"), &g_cfg.m_legit.m_fov, 1, 180);
}

void menu::antiaim_tab()
{

}

void menu::world_esp_tab()
{

}

void menu::player_esp_tab()
{
	ImGui::Text("Player esp settings");
	ImGui::Separator();

	ImGui::Checkbox(_("Name"), &g_cfg.m_esp.m_player[1].m_name);
	ImGui::Checkbox(_("Box"), &g_cfg.m_esp.m_player[1].m_box);
	ImGui::Checkbox(_("Health bar"), &g_cfg.m_esp.m_player[1].m_health_bar);
	ImGui::Checkbox(_("Health text"), &g_cfg.m_esp.m_player[1].m_health_text);
	ImGui::Checkbox(_("Chams"), &g_cfg.m_esp.m_player[1].m_chams);
	ImGui::Checkbox(_("Invisible chams"), &g_cfg.m_esp.m_player[1].m_invisible_chams);
}

void menu::misc_tab()
{
	ImGui::Text(_("Misc"));
	ImGui::Separator();

	ImGui::Checkbox(_("Bunny hop"), &g_cfg.m_misc.m_bunny_hop);
	ImGui::Checkbox(_("Remove scope"), &g_cfg.m_misc.m_no_scope);
	ImGui::Checkbox(_("Remove molotov"), &g_cfg.m_misc.m_remove_molotov);
	ImGui::SliderInt(_("Third person distance"), &g_cfg.m_misc.m_third_person_distance, 0, 250);
	ImGui::Keybind(_("Third person bind"), &g_cfg.m_misc.m_third_person_bind.m_key_selected, &g_cfg.m_misc.m_third_person_bind.m_mode_selected);

	ImGui::Text(_("Configs"));
	ImGui::Separator();

	static ImGuiTextFilter configs_filter;
	configs_filter.Draw(_("##cfg_filter"), 338.f);

	ImGui::Text(_("Available configs"));
	ImGui::Separator();

	//ImGui::Checkbox("test", &cfg::m_text.test_var);
	if (ImGui::Button(_("Save Config"), ImVec2{ 138.f, 19.f }))
	{
		//c_config_manager::get()->save_config("fd");
	}

	if (ImGui::Button(_("Update Configs"), ImVec2{ 138.f, 19.f }))
	{

	}
}