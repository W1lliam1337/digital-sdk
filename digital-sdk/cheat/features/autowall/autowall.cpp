#include "autowall.h"
#include <complex>

void auto_wall::scale_damage(float& damage)
{
	// @xref: https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/server/player.cpp#L1194 
	static auto mp_damage_scale_ct_head = interfaces::m_cvar->find_var(_("mp_damage_scale_ct_head"));
	static auto mp_damage_scale_t_head = interfaces::m_cvar->find_var(_("mp_damage_scale_t_head"));

	static auto mp_damage_scale_ct_body = interfaces::m_cvar->find_var(_("mp_damage_scale_ct_body"));
	static auto mp_damage_scale_t_body = interfaces::m_cvar->find_var(_("mp_damage_scale_t_body"));

	float head_damage_scale = m_fire_bullet_data.m_enter_trace.m_entity->get_team() == team_ct ? mp_damage_scale_ct_head->get_float() : m_fire_bullet_data.m_enter_trace.m_entity->get_team() == team_tt ? mp_damage_scale_t_head->get_float() : 1.0f;
	const float body_damage_scale = m_fire_bullet_data.m_enter_trace.m_entity->get_team() == team_ct ? mp_damage_scale_ct_body->get_float() : m_fire_bullet_data.m_enter_trace.m_entity->get_team() == team_tt ? mp_damage_scale_t_body->get_float() : 1.0f;

	if (m_fire_bullet_data.m_enter_trace.m_entity->has_heavy_armor())
		head_damage_scale *= 0.5f;

	switch (m_fire_bullet_data.m_enter_trace.m_hit_group)
	{
	case hitgroup_head:
		damage *= m_fire_bullet_data.m_weapon_info->m_headshot_multiplier * head_damage_scale;
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

	if (!m_fire_bullet_data.m_enter_trace.m_entity->is_armored(m_fire_bullet_data.m_enter_trace.m_hit_group))
		return;

	// @ida: module: server.dll; sig: 80 BF ? ? ? ? ? F3 0F 10 5C 24 ? F3 0F 10 35 
	float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = m_fire_bullet_data.m_weapon_info->m_armor_ratio * 0.5f;

	if (m_fire_bullet_data.m_enter_trace.m_entity->has_heavy_armor())
	{
		heavy_armor_bonus = 0.25f;
		armor_bonus = 0.33f;
		armor_ratio *= 0.20f;
	}

	float damage_to_health = damage * armor_ratio;
	const float damage_to_armor = (damage - damage_to_health) * (heavy_armor_bonus * armor_bonus);

	if (damage_to_armor > static_cast<float>(m_fire_bullet_data.m_enter_trace.m_entity->get_armour_value()))
		damage_to_health = damage - static_cast<float>(m_fire_bullet_data.m_enter_trace.m_entity->get_armour_value()) / armor_bonus;

	damage = damage_to_health;
}

bool auto_wall::trace_to_exit(c_game_trace* enter_trace, c_game_trace* exit_trace, c_vec3 start, c_vec3& end)
{
	// @ida: https://imgur.com/x3Qe12r 
	// module: server.dll; sig: F3 0F 5C CE F3 0F 11 5D ?
	
	auto distance = 0.0f;
	int v13{};

	while (distance <= 90.0f)
	{
		distance += 4.0f;

		end = start + m_fire_bullet_data.m_direction * distance;
		
		if (!v13)
			v13 = interfaces::m_trace->get_point_contents(end, 0x4600400B, nullptr);

		c_vec3 cl_end = end - m_fire_bullet_data.m_direction * 4.0f;

		const auto contents = interfaces::m_trace->get_point_contents(end, 0x4600400B, nullptr);
		if (contents & 0x600400B && (contents & 0x40000000 || v13 == contents))
			continue;

		static auto trace_filter_simple = utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F0 83 EC 7C 56 52")) + 0x3D;
		std::uintptr_t filt[4] = {
			*reinterpret_cast<std::uintptr_t*>(trace_filter_simple),
			reinterpret_cast<std::uintptr_t>(ctx::local()),
			0,
			0
		};

		interfaces::m_trace->trace_line(end, cl_end, 0x4600400B, ctx::local() /*reinterpret_cast<c_base_player*>(filt[1])*/, 0, exit_trace);

		if (static auto var = interfaces::m_cvar->find_var(_("sv_clip_penetration_traces_to_players"));
			var->get_bool())
		{
			clip_trace_to_players(end, cl_end, reinterpret_cast<c_trace_filter*>(filt), exit_trace, -60.0f);
		}

		if (exit_trace->m_start_solid && exit_trace->m_surface.m_flags & surf_hitbox)
		{
			ray_t ray;
			ray.init(end, start);

			interfaces::m_trace->trace_ray(ray, 0x600400B, reinterpret_cast<c_trace_filter*>(filt), exit_trace);

			if (exit_trace->did_hit() && !exit_trace->m_start_solid)
			{
				end = exit_trace->m_end;
				return true;
			}

			continue;
		}

		if (exit_trace->did_hit() && !exit_trace->m_start_solid)
		{
			if (enter_trace->m_entity->is_breakable() && exit_trace->m_entity->is_breakable())
				return true;

			if (enter_trace->m_surface.m_flags & surf_nodraw || !(exit_trace->m_surface.m_flags & surf_nodraw) && exit_trace->m_plane.m_normal.dot(m_fire_bullet_data.m_direction) <= 1.0f)
			{
				start -= m_fire_bullet_data.m_direction * exit_trace->m_fraction * 4.0f;
				return true;
			}

			continue;
		}

		if (!exit_trace->did_hit() || exit_trace->m_start_solid)
		{
			if (enter_trace->m_entity != nullptr && enter_trace->m_entity->get_index() > 0 && enter_trace->m_entity->is_breakable())
			{
				exit_trace = enter_trace;
				exit_trace->m_end = end + m_fire_bullet_data.m_direction;
				return true;
			}
		}
	}

	/*if ( !c_trace::did_hit_non_world_entity(v18) || !IsBreakableEntity(*(v18 + 76)) )
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

bool auto_wall::handle_bullet_penetration()
{
	// @ida:
	// module: server.dll; sig: 39 07 75 1C;
	// module: client.dll: sig: E8 ? ? ? ? 83 C4 40 84 C0;
	

	static auto ff_damage_reduction = interfaces::m_cvar->find_var(_("ff_damage_reduction_bullets"));
	static auto damage_reduction = ff_damage_reduction->get_float();

	static auto ff_damage_bullet_penetration = interfaces::m_cvar->find_var(_("ff_damage_bullet_penetration"));
	const auto damage_bullet_penetration = ff_damage_bullet_penetration->get_float();

	const bool solid_surf = m_fire_bullet_data.m_enter_trace.m_contents >> 3 & contents_solid;
	const bool light_surf = m_fire_bullet_data.m_enter_trace.m_surface.m_flags >> 7 & surf_light;

	const int enter_material = interfaces::m_physics_surface_props->get_surface_data(m_fire_bullet_data.m_enter_trace.m_surface.m_surface_props)->m_game.m_material;

	if (!light_surf && !solid_surf && enter_material != char_tex_grate && enter_material != char_tex_glass)
		return false;

	if (m_fire_bullet_data.m_penetrate_count <= 0 || m_fire_bullet_data.m_weapon_info->m_penetration <= 0.0f)
		return false;

	if (c_vec3 end; !trace_to_exit(&m_fire_bullet_data.m_enter_trace, &m_fire_bullet_data.m_exit_trace, m_fire_bullet_data.m_enter_trace.m_end, end)
		&& !(interfaces::m_trace->get_point_contents(end, 0x600400B) & 0x600400B))
	{
		return false;
	}

	float dam_lost_percent;
	float cb_penetration_modifier;

	const float enter_penetration_modifier = interfaces::m_physics_surface_props->get_surface_data(m_fire_bullet_data.m_enter_trace.m_surface.m_surface_props)->m_game.m_penetration_modifier;
	const float exit_penetration_modifier = interfaces::m_physics_surface_props->get_surface_data(m_fire_bullet_data.m_exit_trace.m_surface.m_surface_props)->m_game.m_penetration_modifier;

	const auto player = m_fire_bullet_data.m_enter_trace.m_entity;

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
	else if (enter_material == char_tex_flesh && player->get_team() != ctx::local()->get_team() && damage_reduction == 0.f)
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

	const int exit_material = interfaces::m_physics_surface_props->get_surface_data(m_fire_bullet_data.m_exit_trace.m_surface.m_surface_props)->m_game.m_material;
	if (enter_material == exit_material)
	{
		if (exit_material == char_tex_wood || exit_material == char_tex_cardboard)
			cb_penetration_modifier = 3;
		else if (exit_material == char_tex_plastic)
			cb_penetration_modifier = 2;
	}

	const float trace_distance = (m_fire_bullet_data.m_exit_trace.m_end - m_fire_bullet_data.m_enter_trace.m_end).length_sqr();
	const float modifier = cb_penetration_modifier > 0.0f ? 1.0f / cb_penetration_modifier : 0.0f;
	const float lost_damage = m_fire_bullet_data.m_current_damage * dam_lost_percent
		+ (m_fire_bullet_data.m_weapon_info->m_penetration > 0.0f ? 3.75f / m_fire_bullet_data.m_weapon_info->m_penetration : 0.0f)
		* (modifier * 3.0f) + modifier * trace_distance / 24.0f;

	if (lost_damage > m_fire_bullet_data.m_current_damage)
		return false;

	if (lost_damage > 0.f)
		m_fire_bullet_data.m_current_damage -= lost_damage;

	if (m_fire_bullet_data.m_current_damage < 1.f)
		return false;

	m_fire_bullet_data.m_position = m_fire_bullet_data.m_exit_trace.m_end;
	--m_fire_bullet_data.m_penetrate_count;

	return true;
}

bool auto_wall::simulate_fire_bullet()
{
	// @ida:
	// module: client.dll; sig: (+0x1805) E8 ? ? ? ? 46 3B B3 ? ? ? ? 7C 8C ;
	// module: server.dll; sig: 8B 01 F3 0F 11 84 24 ? ? ? ? F3 0F 10 84 24 ? ? ? ? ;
	
	c_trace_filter filter;
	filter.m_skip = ctx::local();

	while (m_fire_bullet_data.m_penetrate_count > 0 && m_fire_bullet_data.m_current_damage >= 1.0f)
	{
		const float remains = m_fire_bullet_data.m_weapon_info->m_range - m_fire_bullet_data.m_current_distance;
		auto end = m_fire_bullet_data.m_position + m_fire_bullet_data.m_direction * remains;

		ray_t ray;
		ray.init(m_fire_bullet_data.m_position, end);

		interfaces::m_trace->trace_ray(ray, mask_shot_hull | contents_hitbox, &filter, &m_fire_bullet_data.m_enter_trace);

		const auto enter_surface_data = interfaces::m_physics_surface_props->get_surface_data(m_fire_bullet_data.m_enter_trace.m_surface.m_surface_props);

		clip_trace_to_players(m_fire_bullet_data.m_position, end + m_fire_bullet_data.m_direction * 40.0f, &filter, &m_fire_bullet_data.m_enter_trace);

		if (m_fire_bullet_data.m_enter_trace.m_fraction == 1.0f)
			break;

		m_fire_bullet_data.m_current_distance += m_fire_bullet_data.m_enter_trace.m_fraction * remains;
		m_fire_bullet_data.m_current_damage *= std::powf(m_fire_bullet_data.m_weapon_info->m_range_modifier, m_fire_bullet_data.m_current_distance / 500.0f);

		if (m_fire_bullet_data.m_current_distance > 3000.f || enter_surface_data->m_game.m_penetration_modifier < 0.1f)
			break;

		if (m_fire_bullet_data.m_enter_trace.m_hit_group != hitgroup_generic && m_fire_bullet_data.m_enter_trace.m_hit_group != hitgroup_gear && m_fire_bullet_data.m_enter_trace.m_entity->get_team() != ctx::local()->get_team())
		{
			scale_damage(m_fire_bullet_data.m_current_damage);
			return true;
		}

		if (!handle_bullet_penetration())
			break;
	}

	return false;
}

void i_engine_trace::trace_line(const c_vec3 src, const c_vec3 dst, const int mask, i_handle_entity* entity, const int collision_group, c_game_trace* trace)
{
	// @ida: https://imgur.com/62wteAl 
	// module: server; sig: E8 ? ? ? ? 8B 45 2C
	
	static auto trace_filter_simple = utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F0 83 EC 7C 56 52")) + 0x3D;
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

void auto_wall::clip_trace_to_players(const c_vec3& abs_start, const c_vec3& abs_end, c_trace_filter* filter, c_game_trace* trace, const float min_range)
{
	// @note: i don't know if signature can be used, because all sources just copy the valve code and paste it to yourself 
	//static const auto clip_trace_to_players_fn = reinterpret_cast<void(__thiscall*)(i_handle_entity*, c_vec3, c_vec3, unsigned int, i_trace_filter*, trace_t*, float)>(utils::sig(ctx::m_modules.m_client_dll, _("E8 ? ? ? ? 83 C4 18 8A 56 37")));
	//return clip_trace_to_players_fn(entity, abs_start, abs_end, mask, filter, trace, min_range);

	c_game_trace game_trace = { };

	ray_t ray;
	ray.init(abs_start, abs_end);

	for (int i = 1; i < interfaces::m_globals->m_max_clients; i++)
	{
		const auto entity = static_cast<c_base_player*>(interfaces::m_entity_list->get_client_entity(i));

		if (!entity || !entity->is_alive() || entity->is_dormant())
			continue;

		if (!filter && !filter->should_hit_entity(entity, mask_shot_hull | contents_hitbox))
			continue;

		const auto collideable = entity->get_collideable();

		if (!collideable)
			continue;

		const auto& min = collideable->obb_mins();
		const auto& max = collideable->obb_maxs();

		const auto center = (max + min) / 2.0f;
		const auto position = center + entity->get_origin();

		auto to = position - abs_start;
		auto direction = abs_end - abs_start;
		const auto length = direction.normalize();

		const auto range_along = direction.dot(to);
		float range;

		if (range_along < 0.0f)
			range = -to.length();
		else if (range_along > length)
			range = -(position - abs_end).length();
		else
			range = (position - (direction * range_along + abs_start)).length();

		if (range < min_range || range > 60.0f)
			continue;

		interfaces::m_trace->clip_ray_to_entity(ray, mask_shot_hull | contents_hitbox, entity, &game_trace);

		if (game_trace.m_fraction > trace->m_fraction)
			std::memcpy(trace, &game_trace, sizeof(c_game_trace));
	}
}

float auto_wall::get_damage(const c_vec3& point)
{
	c_vec3 vec_direction;
	qangle_t ang_direction;

	const auto shoot_pos = ctx::local()->get_shoot_pos();

	math::vector_angles(point - shoot_pos, ang_direction);
	math::angle_vectors(ang_direction, &vec_direction);
	vec_direction.normalize_in_place();

	m_fire_bullet_data.m_position = shoot_pos;
	m_fire_bullet_data.m_direction = vec_direction;

	const auto weapon = ctx::local()->get_active_weapon();
	if (!weapon)
		return 0.0f;

	const auto weapon_info = weapon->get_weapon_data();
	if (!weapon_info)
		return 0.0f;

	m_fire_bullet_data.m_weapon_info = weapon_info;
	m_fire_bullet_data.m_weapon = weapon;
	m_fire_bullet_data.m_penetrate_count = 4;
	m_fire_bullet_data.m_current_damage = m_fire_bullet_data.m_weapon_info->m_damage;

	if (simulate_fire_bullet())
		return m_fire_bullet_data.m_current_damage;
	return 0.0f;
}
