#pragma once
#include "../../../../includes.h"
#include "../../../../sdk/sdk.h"

enum e_esp_type
{
	both,
	player,
	team,
	local,
};

namespace player_esp
{
	inline int m_type{};

	int get_type(c_base_player* player);
	void draw();
	void render_box(RECT bbox, c_base_player* player);
	void render_name(RECT bbox, c_base_player* player);
	void render_health_bar(RECT bbox, c_base_player* player);
	void render_weapon(RECT bbox, c_base_player* player);
};