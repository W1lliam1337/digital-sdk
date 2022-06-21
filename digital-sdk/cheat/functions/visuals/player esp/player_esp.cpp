#include "player_esp.h"

/* @fix-me: funny and stupid code */
bool get_bounding_box(c_base_player* entity, bbox_t& box)
{
	const auto collideable = entity->get_collideable();
	const auto &coordinate_frame = entity->get_coordinate_frame();

	const auto &min = collideable->obb_mins();
	const auto &max = collideable->obb_maxs();

	const vec3_t points[8] =
	{
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	vec3_t points_transformed[8];

	for (auto i = 0; i < 8; i++)
		c_math::vector_transform(points[i], coordinate_frame, points_transformed[i]);

	vec3_t flb;
	vec3_t brt;
	vec3_t blb;
	vec3_t frt;
	vec3_t frb;
	vec3_t brb;
	vec3_t blt;
	vec3_t flt;

	const auto b_flb = c_math::world_to_screen(points_transformed[3], flb);
	const auto b_brt = c_math::world_to_screen(points_transformed[5], brt);
	const auto b_blb = c_math::world_to_screen(points_transformed[0], blb);
	const auto b_frt = c_math::world_to_screen(points_transformed[4], frt);
	const auto b_frb = c_math::world_to_screen(points_transformed[2], frb);
	const auto b_brb = c_math::world_to_screen(points_transformed[1], brb);
	const auto b_blt = c_math::world_to_screen(points_transformed[6], blt);
	const auto b_flt = c_math::world_to_screen(points_transformed[7], flt);

	if (!b_flb && !b_brt && !b_blb && !b_frt && !b_frb && !b_brb && !b_blt && !b_flt)
		return false;

	const vec3_t arr[8] =
	{
		flb,
		brt,
		blb,
		frt,
		frb,
		brb,
		blt,
		flt
	};

	auto left = flb.x;
	auto top = flb.y;
	auto right = flb.x;
	auto bottom = flb.y;

	for (auto i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	box.x = left;
	box.y = bottom;
	box.w = right - left;
	box.h = top - bottom;

	return true;
}

int c_player_esp::get_type(c_base_player* player)
{
	if (player->is_player() && player != g_sdk.m_local() && player->get_team() != g_sdk.m_local()->get_team())
		return esp_type_t::player;

	if (player == g_sdk.m_local() && g_sdk.m_interfaces.m_input->m_camera_in_third_person)
		return esp_type_t::local;

	if (player->get_team() == g_sdk.m_local()->get_team())
		return esp_type_t::team;

	return esp_type_t::both;
}

void c_player_esp::draw()
{
	for (int i = 0; i < g_sdk.m_interfaces.m_globals->m_max_clients; i++)
	{
		const auto entity = static_cast<c_base_player*>(g_sdk.m_interfaces.m_entity_list->get_client_entity(i));

		if (!entity || !entity->is_alive() || !entity->is_player())
			continue;

		m_type = get_type(entity);

		if (m_type != esp_type_t::player)
			continue;

		bbox_t bbox;
		if (!get_bounding_box(entity, bbox))
			return;

		render_box(bbox, entity);
		render_name(bbox, entity);
		render_health_bar(bbox, entity);
	}
}

void c_player_esp::render_box(const bbox_t bbox, c_base_player* player)
{
	c_render::rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color::black);
	c_render::rect(bbox.x, bbox.y, bbox.w, bbox.h, color::white);
	c_render::rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color::black);
}

void c_player_esp::render_name(const bbox_t bbox, const c_base_player* player)
{
	player_info_t p_info{};
	g_sdk.m_interfaces.m_engine->get_player_info(player->get_index(), &p_info);
	const std::string name = p_info.m_name;

	c_render::text(g_sdk.m_fonts.m_esp, bbox.x + (bbox.w / 2), bbox.y - 13, color(150, 150, 150, 255), hfont_centered_x, name.c_str());
}

void c_player_esp::render_health_bar(const bbox_t bbox, const c_base_player* player) const
{
	c_render::rect(bbox.x - 7, bbox.y - 1, 4, bbox.h + 2, color::black);
	const int pixel_value = player->get_health() * bbox.h / 100;
	c_render::rect_filled(bbox.x - 6, bbox.y + bbox.h - pixel_value, 2, pixel_value, color(0, 255, 0));
	c_render::text(g_sdk.m_fonts.m_esp, bbox.x - 9, bbox.y + bbox.h - pixel_value - 5, color(150,150,150,255), hfont_centered_x | hfont_centered_y, std::to_string(player->get_health()).c_str());
}