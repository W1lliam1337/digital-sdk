#pragma once
#include "../../../sdk/sdk.hpp"

enum esp_type_t
{
	both,
	player,
	team,
	local,
};

class c_player_esp : public c_singleton<c_player_esp>
{
	int m_type{};
public:
	static int get_type(c_base_player* player);
	void draw();
	static void render_box(RECT bbox, c_base_player* player);
	static void render_name(RECT bbox, const c_base_player* player);
	void render_health_bar(RECT bbox, const c_base_player* player) const;
};

