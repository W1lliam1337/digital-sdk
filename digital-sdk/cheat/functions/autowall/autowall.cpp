#include "autowall.h"
#include <complex>

void c_autowall::scale_damage(fire_bullet_data_t& data, float& damage) const
{
	/* @ref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/server/player.cpp#L1194 */
	static auto is_armored = [](const c_base_player* e_player, const int e_hitgroup)
	{
		if (e_player->get_armour_value() <= 0)
			return false;

		/* if the hitgroup is headand the player has a helment, return true
		 * otherwise only return true if the hitgroup is not generic / legs / gear
		*/
		if (e_hitgroup == hitgroup_head && e_player->has_helmet())
			return true;
		if (e_hitgroup >= hitgroup_chest && e_hitgroup <= hitgroup_rightarm)
			return true;

		return false;
	};

	static auto* mp_damage_scale_ct_head = g_sdk.m_interfaces.m_cvar->find_var(_("mp_damage_scale_ct_head"));
	static auto* mp_damage_scale_t_head = g_sdk.m_interfaces.m_cvar->find_var(_("mp_damage_scale_t_head"));

	static auto* mp_damage_scale_ct_body = g_sdk.m_interfaces.m_cvar->find_var(_("mp_damage_scale_ct_body"));
	static auto* mp_damage_scale_t_body = g_sdk.m_interfaces.m_cvar->find_var(_("mp_damage_scale_t_body"));

	float head_damage_scale = data.m_enter_trace.m_entity->get_team() == team_ct ? mp_damage_scale_ct_head->get_float() : data.m_enter_trace.m_entity->get_team() == team_tt ? mp_damage_scale_t_head->get_float() : 1.0f;
	const float body_damage_scale = data.m_enter_trace.m_entity->get_team() == team_ct ? mp_damage_scale_ct_body->get_float() : data.m_enter_trace.m_entity->get_team() == team_tt ? mp_damage_scale_t_body->get_float() : 1.0f;

	if (data.m_enter_trace.m_entity->has_heavy_armor())
		head_damage_scale *= 0.5f;

	switch (data.m_enter_trace.m_hit_group)
	{
	case hitgroup_head:
		damage *= data.m_weapon_info->m_headshot_multiplier * head_damage_scale;
		break;
	case hitgroup_chest:
	case hitgroup_leftarm:
	case hitgroup_rightarm:
	case hitgroup_neck:
		damage *= body_damage_scale;
		break;
	case hitgroup_stomach:
		damage *= 1.25f * body_damage_scale;
		break;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		damage *= 0.75f * body_damage_scale;
		break;
	default:
		break;
	}

	if (!is_armored(data.m_enter_trace.m_entity, data.m_enter_trace.m_hit_group))
		return;

	/* @ida: module: server.dll; sig: 80 BF ? ? ? ? ? F3 0F 10 5C 24 ? F3 0F 10 35 */
	float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = data.m_weapon_info->m_armor_ratio * 0.5f;

	if (data.m_enter_trace.m_entity->has_heavy_armor())
	{
		heavy_armor_bonus = 0.25f;
		armor_bonus = 0.33f;
		armor_ratio *= 0.20f;
	}

	float damage_to_health = damage * armor_ratio;
	const float damage_to_armor = (damage - damage_to_health) * (heavy_armor_bonus * armor_bonus);

	if (damage_to_armor > static_cast<float>(data.m_enter_trace.m_entity->get_armour_value()))
		damage_to_health = damage - static_cast<float>(data.m_enter_trace.m_entity->get_armour_value()) / armor_bonus;

	damage = damage_to_health;
}

bool c_autowall::trace_to_exit(c_game_trace* enter_trace, c_game_trace* exit_trace, const vec3_t& pos, vec3_t& dir) const
{
	/* @ida: https://imgur.com/x3Qe12r
	 * module: server.dll; sig: F3 0F 5C CE F3 0F 11 5D ?
	*/

	auto distance = 0.0f;
	int v13{};

	while (distance <= 90.0f)
	{
		distance += 4.0f;

		vec3_t cl_start = pos + dir * distance;
		vec3_t cl_end = cl_start - dir * 4.0f;

		if (!v13)
			v13 = g_sdk.m_interfaces.m_trace->get_point_contents(cl_start, 0x4600400B, nullptr);

		auto contents = g_sdk.m_interfaces.m_trace->get_point_contents(cl_start, 0x4600400B, nullptr);
		if (contents & 0x600400B && (contents & 0x40000000 || v13 == contents))
			continue;

		static auto trace_filter_simple = c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F0 83 EC 7C 56 52")) + 0x3D;
		std::uintptr_t filt[4] = {
			*reinterpret_cast<std::uintptr_t*>(trace_filter_simple),
			reinterpret_cast<std::uintptr_t>(g_sdk.m_local()),
			0,
			0
		};

		g_sdk.m_interfaces.m_trace->trace_line(cl_end, cl_start, 0x4600400B, g_sdk.m_local() /*reinterpret_cast<c_base_player*>(filt[1])*/, 0, exit_trace);

		if (static auto var = g_sdk.m_interfaces.m_cvar->find_var(_("sv_clip_penetration_traces_to_players"));
			var->get_bool())
		{
			clip_trace_to_players(cl_end, cl_start, 0x4600400B, reinterpret_cast<i_trace_filter*>(filt), exit_trace);
		}

		if (exit_trace->m_start_solid && exit_trace->m_surface.flags & SURF_HITBOX)
		{
			ray_t ray;
			ray.init(cl_start, cl_end);

			g_sdk.m_interfaces.m_trace->trace_ray(ray, 0x600400B, reinterpret_cast<i_trace_filter*>(filt), exit_trace);

			if (exit_trace->did_hit() && !exit_trace->m_start_solid)
			{
				cl_start = exit_trace->m_end;
				return true;
			}

			continue;
		}

		if (exit_trace->did_hit() && !exit_trace->m_start_solid)
		{
			if (enter_trace->m_entity->is_breakable() && exit_trace->m_entity->is_breakable())
				return true;

			if (enter_trace->m_surface.flags & SURF_NODRAW || !(exit_trace->m_surface.flags & SURF_NODRAW) && exit_trace->m_plane.normal.dot(dir) <= 1.0f)
			{
				cl_start -= dir * exit_trace->m_fraction * 4.0f;
				return true;
			}

			continue;
		}

		if (!exit_trace->did_hit() || exit_trace->m_start_solid)
		{
			if (enter_trace->m_entity != nullptr && enter_trace->m_entity->get_index() != 0 && enter_trace->m_entity->is_breakable())
			{
				exit_trace = enter_trace;
				exit_trace->m_end = cl_start + dir;
				return true;
			}
		}
	}

	/* if ( !c_trace::did_hit_non_world_entity(v18) || !IsBreakableEntity(*(v18 + 76)) )
	  {
	LABEL_24:
	    vec_direction_x = a6;
	    vec_direction_y = a7;
	    vec_direction_z = a8;
	    goto LABEL_25;
	  }
	*/

	return false;
}

bool c_autowall::handle_bullet_penetration(fire_bullet_data_t& data) const
{
	/* @ida:
	 * module: server.dll; sig: 39 07 75 1C;
	 * module: client.dll: sig: E8 ? ? ? ? 83 C4 40 84 C0;
	*/

	static auto ff_damage_reduction = g_sdk.m_interfaces.m_cvar->find_var(_("ff_damage_reduction_bullets"));
	static auto damage_reduction = ff_damage_reduction->get_float();

	static auto ff_damage_bullet_penetration = g_sdk.m_interfaces.m_cvar->find_var(_("ff_damage_bullet_penetration"));
	const auto damage_bullet_penetration = ff_damage_bullet_penetration->get_float();

	const bool solid_surf = data.m_enter_trace.m_contents >> 3 & CONTENTS_SOLID;
	const bool light_surf = data.m_enter_trace.m_surface.flags >> 7 & SURF_LIGHT;

	const int enter_material = g_sdk.m_interfaces.m_physics_surface_props->get_surface_data(data.m_enter_trace.m_surface.surfaceProps)->m_game.m_material;

	if (!light_surf && !solid_surf && enter_material != char_tex_grate && enter_material != char_tex_glass)
		return false;

	if (data.m_penetrate_count <= 0 || data.m_weapon_info->m_penetration <= 0.0f)
		return false;

	if (!trace_to_exit(&data.m_enter_trace, &data.m_exit_trace, data.m_enter_trace.m_end, data.m_direction) 
		&& !(g_sdk.m_interfaces.m_trace->get_point_contents(data.m_enter_trace.m_end, 0x600400B) & 0x600400B))
	{
		return false;
	}

	float dam_lost_percent;
	float cb_penetration_modifier;

	const float enter_penetration_modifier = g_sdk.m_interfaces.m_physics_surface_props->get_surface_data(data.m_enter_trace.m_surface.surfaceProps)->m_game.m_penetration_modifier;
	const float exit_penetration_modifier = g_sdk.m_interfaces.m_physics_surface_props->get_surface_data(data.m_exit_trace.m_surface.surfaceProps)->m_game.m_penetration_modifier;

	const auto player = data.m_enter_trace.m_entity;

	if (!player)
		return false;

	if (enter_material == char_tex_grate || enter_material == char_tex_glass)
	{
		cb_penetration_modifier = 3.f;
		dam_lost_percent = 0.05f;
	}
	else if (solid_surf || light_surf)
	{
		cb_penetration_modifier = 1.f;
		dam_lost_percent = 0.16f;
	}
	else if (enter_material == char_tex_flesh && player->get_team() != g_sdk.m_local()->get_team() && damage_reduction == 0.f)
	{
		if (damage_bullet_penetration == 0.f)
			return false;

		cb_penetration_modifier = damage_bullet_penetration;
		dam_lost_percent = damage_bullet_penetration;
	}
	else
	{
		cb_penetration_modifier = (enter_penetration_modifier + exit_penetration_modifier) * 0.5f;
		dam_lost_percent = 0.16f;
	}

	const int exit_material = g_sdk.m_interfaces.m_physics_surface_props->get_surface_data(data.m_exit_trace.m_surface.surfaceProps)->m_game.m_material;
	if (enter_material == exit_material)
	{
		if (exit_material == char_tex_wood || exit_material == char_tex_cardboard)
			cb_penetration_modifier = 3;
		else if (exit_material == char_tex_plastic)
			cb_penetration_modifier = 2;
	}

	const float trace_distance = (data.m_exit_trace.m_end - data.m_enter_trace.m_end).length_sqr();
	const float modifier = cb_penetration_modifier > 0.0f ? 1.0f / cb_penetration_modifier : 0.0f;
	const float lost_damage = data.m_current_damage * dam_lost_percent
		+ (data.m_weapon_info->m_penetration > 0.0f ? 3.75f / data.m_weapon_info->m_penetration : 0.0f)
		* (modifier * 3.0f) + modifier * trace_distance / 24.0f;

	if (lost_damage > data.m_current_damage)
		return false;

	if (lost_damage > 0.f)
		data.m_current_damage -= lost_damage;

	if (data.m_current_damage < 1.f)
		return false;

	data.m_position = data.m_exit_trace.m_end;
	--data.m_penetrate_count;

	return true;
}

bool c_autowall::simulate_fire_bullet(fire_bullet_data_t& data) const
{
	/* @ida:
	 * module: client.dll; sig: (+0x1805) E8 ? ? ? ? 46 3B B3 ? ? ? ? 7C 8C ;
	 * module: server.dll; sig: 8B 01 F3 0F 11 84 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ? ;
	*/
	float max_range = data.m_weapon_info->m_range;

	data.m_penetrate_count = 4;
	data.m_current_damage = data.m_weapon_info->m_damage;

	c_trace_filter filter;
	filter.pSkip = g_sdk.m_local();

	while (data.m_penetrate_count > 0 && data.m_current_damage >= 1.0f)
	{
		max_range -= data.m_current_distance;
		const auto end = data.m_position + data.m_direction * max_range;

		ray_t ray;
		ray.init(data.m_position, end);

		g_sdk.m_interfaces.m_trace->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.m_enter_trace);

		clip_trace_to_players(data.m_position, end + data.m_direction * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.m_enter_trace);

		const auto enter_surface_data = g_sdk.m_interfaces.m_physics_surface_props->get_surface_data(data.m_enter_trace.m_surface.surfaceProps);

		if (data.m_enter_trace.m_fraction == 1.0f)
			break;

		data.m_current_distance += data.m_enter_trace.m_fraction * max_range;
		data.m_current_damage *= std::powf(data.m_weapon_info->m_range_modifier, data.m_current_distance / 500.0f);

		if (data.m_current_distance > 3000.f || enter_surface_data->m_game.m_penetration_modifier < 0.1f)
			break;

		if (data.m_enter_trace.m_hit_group != hitgroup_generic && data.m_enter_trace.m_hit_group != hitgroup_gear && data.m_enter_trace.m_entity->get_team() != g_sdk.m_local()->get_team())
		{
			scale_damage(data, data.m_current_damage);
			return true;
		}

		if (!handle_bullet_penetration(data))
			break;
	}

	return false;
}

void c_engine_trace::trace_line(const vec3_t src, const vec3_t dst, const int mask, i_handle_entity* entity, const int collision_group, c_game_trace* trace)
{
	/* @ida: https://imgur.com/62wteAl
	 * module: server; sig: E8 ? ? ? ? 8B 45 2C
	*/

	static auto trace_filter_simple = c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("55 8B EC 83 E4 F0 83 EC 7C 56 52")) + 0x3D;
	std::uintptr_t filt[4] = {
		*reinterpret_cast<std::uintptr_t*>(trace_filter_simple),
		reinterpret_cast<std::uintptr_t>(entity),
		static_cast<uintptr_t>(collision_group),
		0
	};

	ray_t ray;
	ray.init(src, dst);

	return trace_ray(ray, mask, reinterpret_cast<c_trace_filter*>(filt), trace);
}

void c_autowall::clip_trace_to_players(const vec3_t& abs_start, const vec3_t& abs_end, const unsigned int mask, i_trace_filter* filter, c_game_trace* trace)
{
	/* @note: i don't know if signature can be used, because all sources just copy the valve code and paste it to yourself */
	//static auto clip_trace_to_players_fn = reinterpret_cast<void(__thiscall*)(i_handle_entity*, vec3_t, vec3_t, unsigned int, i_trace_filter*, trace_t*, float)>(c_utils::find_sig(g_sdk.m_modules.m_client_dll, _("E8 ? ? ? ? 83 C4 18 8A 56 37")));
	//return clip_trace_to_players_fn(entity, abs_start, abs_end, mask, filter, trace, min_range);

	c_game_trace game_trace = { };
	float smallest_fraction = trace->m_fraction;

	ray_t ray;
	ray.init(abs_start, abs_end);

	for (int i = 1; i < g_sdk.m_interfaces.m_globals->m_max_clients; i++)
	{
		const auto entity = static_cast<c_base_player*>(g_sdk.m_interfaces.m_entity_list->get_client_entity(i));

		if (!entity || !entity->is_alive() || entity->is_dormant())
			continue;

		if (!filter && !filter->should_hit_entity(entity, mask))
			continue;

		const auto collideable = entity->get_collideable();

		if (!collideable)
			continue;

		const auto &min = collideable->obb_mins();
		const auto &max = collideable->obb_maxs();

		const auto center = (max + min) / 2.0f;
		const auto position = center + entity->get_origin();

		auto to = position - abs_start;
		auto direction = abs_end - abs_start;
		const auto length = direction.normalize();

		const auto range_along = direction.dot(to);
		auto range = 0.0f;

		if (range_along < 0.0f)
			range = -to.length();
		else if (range_along > length)
			range = -(position - abs_end).length();
		else
			range = (position - (direction * range_along + abs_start)).length();

		if (range < 0.0f || range > 60.0f)
			continue;

		g_sdk.m_interfaces.m_trace->clip_ray_to_entity(ray, mask | CONTENTS_HITBOX, entity, &game_trace);

		if (game_trace.m_fraction < smallest_fraction)
		{
			*trace = game_trace;
			smallest_fraction = game_trace.m_fraction;
		}
	}
}

float c_autowall::get_damage(const vec3_t& point, fire_bullet_data_t* data) const
{
	fire_bullet_data_t fire_bullet_data{};

	vec3_t vec_direction;
	qangle_t ang_direction;

	c_math::get()->vector_angles(point - g_sdk.m_local_data.m_shoot_pos, ang_direction);
	c_math::get()->angle_vectors(ang_direction, &vec_direction);
	vec_direction.normalize_in_place();

	fire_bullet_data.m_position = g_sdk.m_local_data.m_shoot_pos;
	fire_bullet_data.m_direction = vec_direction;

	if (data)
		*data = fire_bullet_data;

	return simulate_fire_bullet(fire_bullet_data) ? fire_bullet_data.m_current_damage : -1.0f;
}