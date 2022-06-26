#pragma once
#include "../../sdk/sdk.hpp"
#include "../../dependencies/config manager/config_manager.h"

enum e_menu : int
{
	rage_tab,
	aa_tab,
	world_esp_tab,
	player_esp_tab,
	misc_tab
};

class c_menu : public c_singleton<c_menu>
{
public:
	static bool init_style() noexcept;
	static void color_edit(const char* label, c_color* color);
	void render() const;
	static void rage_tab();
	static void antiaim_tab();
	static void world_esp_tab();
	static void player_esp_tab();
	static void misc_tab();
};

