#include "menu.h"
#include <cmath>

bool c_menu::init_style() noexcept
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

void c_menu::render()
{
	auto& style = ImGui::GetStyle();
	const auto& io = ImGui::GetIO();

	static float alpha = 0.f;

	const auto& draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(ImVec2{ 0.f, 0.f }, io.DisplaySize, ImColor{ 0.f, 0.f, 0.f, alpha });

	if (!g_sdk.m_menu_data.m_is_menu_opened)
	{
		alpha = std::fmax(alpha - io.DeltaTime * 3.0f, 0.f);
		style.Alpha = std::fmax(style.Alpha - io.DeltaTime * 6.0f, 0.f);
		return;
	}

	alpha = std::fmin(alpha + io.DeltaTime * 3.0f, 0.5f);
	style.Alpha = std::fmin(style.Alpha + io.DeltaTime * 6.0f, 1.f);

	static std::uint32_t selected_tab = 0;
	ImGui::SetNextWindowSize(ImVec2{ 412.f, 515.f }, ImGuiCond_Once);
	ImGui::Begin(_("digital csgo sdk"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::BeginChild("Tabs Panel", ImVec2{ 150, 0 }, true);
	{
		static constexpr const char* tabs[]{
			"Rage",
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

void c_menu::rage_tab()
{
	ImGui::Text("Rage settings");
	ImGui::Separator();
}

void c_menu::antiaim_tab()
{
	
}

void c_menu::world_esp_tab()
{
	
}

void c_menu::player_esp_tab()
{
	
}

void c_menu::misc_tab()
{
	ImGui::Text("Configs");
	ImGui::Separator();

	static ImGuiTextFilter configs_filter;
	configs_filter.Draw("##cfg_filter", 338.f);

	ImGui::Text("Available configs");
	ImGui::Separator();

	//ImGui::Checkbox("test", &g_cfg.m_text.test_var);
	if (ImGui::Button("Save Config", ImVec2{ 338.f, 19.f }))
	{
		//c_config_manager::get()->save_config("fd");
	}

	if (ImGui::Button("Update Configs", ImVec2{ 338.f, 19.f }))
	{

	}
}