#include "player_esp.h"

/* @fix-me: funny and stupid code */
RECT get_bounding_box(c_base_player* entity)
{
	constexpr RECT rect{};

	const auto collideable = entity->get_collideable();
	const auto& coordinate_frame = entity->get_coordinate_frame();

	const auto& min = collideable->obb_mins();
	const auto& max = collideable->obb_maxs();

	const vec3_t points[] =
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

	vec3_t screen_points[8] = {};
	for (int i = 0; i < 8; i++)
		if (g_sdk.m_interfaces.m_debug_overlay->screen_position(points_transformed[i], screen_points[i]))
			return rect;

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (auto i = 1; i < 8; i++)
	{
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}


	return {static_cast<long>(left), static_cast<long>(bottom), static_cast<long>(right), static_cast<long>(top)};
}

int c_player_esp::get_type(c_base_player* player)
{
	if (player->is_player() && player != g_sdk.m_local() && player->get_team() != g_sdk.m_local()->get_team())
		return esp_type_t::player;

	if (player == g_sdk.m_local() && g_sdk.m_interfaces.m_input->m_camera_in_third_person)
		return local;

	if (player->get_team() == g_sdk.m_local()->get_team())
		return team;

	return both;
}

void c_player_esp::draw()
{
	for (int i{ 1 }; i < g_sdk.m_interfaces.m_globals->m_max_clients; i++)
	{
		const auto entity = static_cast<c_base_player*>(g_sdk.m_interfaces.m_entity_list->get_client_entity(i));

		if (!entity || !entity->is_alive() || !entity->is_player())
			continue;

		m_type = get_type(entity);

		if (m_type != player)
			continue;

		const auto bbox = get_bounding_box(entity);

		render_box(bbox, entity);
		render_name(bbox, entity);
		render_health_bar(bbox, entity);
	}
}

void c_player_esp::render_box(const RECT bbox, c_base_player* player)
{
	c_render::get()->rect(bbox.left + 1, bbox.top + 1, bbox.right - 1, bbox.bottom - 1, c_color(0.f, 0.f, 0.f));
	c_render::get()->rect(bbox.left - 1, bbox.top - 1, bbox.right + 1, bbox.bottom + 1, c_color(0.f, 0.f, 0.f));
	c_render::get()->rect(bbox.left, bbox.top, bbox.right, bbox.bottom, c_color::white);
}

void c_player_esp::render_name(const RECT bbox, const c_base_player* player)
{
	player_info_t p_info{};
	g_sdk.m_interfaces.m_engine->get_player_info(player->get_index(), &p_info);
	const std::string name = p_info.m_name;

	c_render::get()->text(g_sdk.m_fonts.m_esp, ImVec2(bbox.left + (bbox.right - bbox.left) * 0.5f, bbox.top - 17),
	                      name.c_str(), c_color::white, true);
}

void c_player_esp::render_health_bar(const RECT bbox, const c_base_player* player) const
{
	const auto box_height = static_cast<float>(bbox.bottom - bbox.top);

	const float colored_bar_height = box_height * min(player->get_health(), 100) / 100.0f;
	const float colored_bar_max_height = box_height * 100.0f / 100.0f;

	c_render::get()->rect_filled(bbox.left - 6.0f, bbox.top - 1, bbox.left - 2.0f,
	                             bbox.top + colored_bar_max_height + 1, c_color(0.0f, 0.0f, 0.0f));

	c_render::get()->rect_filled(bbox.left - 5.0f, bbox.top + (colored_bar_max_height - colored_bar_height),
	                             bbox.left - 3.0f, bbox.top + colored_bar_max_height, c_color(0, 255, 0));

	c_render::get()->text(g_sdk.m_fonts.m_esp, ImVec2(bbox.left - 20, bbox.top - 2.0f),
	                      std::to_string(player->get_health()), c_color(0, 255, 0), false, true);
}
