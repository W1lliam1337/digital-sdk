#pragma once
#include "../../../sdk/sdk.hpp"

enum esp_type_t
{
	both,
	player,
	team,
	local,
};

struct bbox_t
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

class c_player_esp : public c_singleton<c_player_esp>
{
	int m_type{};
public:
	static int get_type(c_base_player* player);
	void draw();
	static void render_box(bbox_t bbox, c_base_player* player);
	static void render_name(bbox_t bbox, const c_base_player* player);
	void render_health_bar(bbox_t bbox, const c_base_player* player) const;
};

