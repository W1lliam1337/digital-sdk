#pragma once
#include "../../includes.h"
#include "../../sdk/sdk.h"

enum e_menu : int
{
	rage_tab,
	legit_tab,
	aa_tab,
	world_esp_tab,
	player_esp_tab,
	misc_tab
};

namespace menu
{
	bool init_style() noexcept;
	void color_edit(const char* label, c_color* color);
	void render();
	void rage_tab();
	void legit_tab();
	void antiaim_tab();
	void world_esp_tab();
	void player_esp_tab();
	void misc_tab();
}