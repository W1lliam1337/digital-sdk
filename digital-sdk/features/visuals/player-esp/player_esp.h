#pragma once
#include "../../../includes.h"
#include "../../../sdk/sdk.h"

enum e_esp_type
{
	both,
	player,
	team,
	local,
};

class c_player_esp
{
	int m_type{};
public:
	static int get_type(c_base_player* player);
	void draw();
	void render_box(RECT bbox, c_base_player* player) const;
	void render_name(RECT bbox, const c_base_player* player) const;
	void render_health_bar(RECT bbox, const c_base_player* player) const;
};

inline const auto g_player_esp = std::make_unique<c_player_esp>();
