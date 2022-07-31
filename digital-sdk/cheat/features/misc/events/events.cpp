#include "events.h"
#include "../log-system//logs.h"

void c_events::init() noexcept
{
	interfaces::m_event_manager->add_listener(this, _("player_hurt"), false);
	interfaces::m_event_manager->add_listener(this, _("player_death"), false);
}

void c_events::fire_game_event(i_game_event* event)
{
	if (!event || !ctx::local() || !ctx::local()->is_alive())
		return;

	const auto event_name = event->get_name();

	player_hurt(event, event_name);
	player_death(event, event_name);
}

void c_events::player_hurt(i_game_event* event, const char* event_name) const
{
	const auto user_id = interfaces::m_engine->get_player_for_user_id(event->get_int(_("userid")));
	const auto attacker = interfaces::m_engine->get_player_for_user_id(event->get_int(_("attacker")));

	if (strcmp(event_name, _("player_hurt")))
		return;

	const auto hurt_player = static_cast<c_base_player*>(interfaces::m_entity_list->get_client_entity(user_id));

	if (!hurt_player || !hurt_player->is_player())
		return;

	auto get_hitbox_by_hitgroup = [](const int hitgroup) -> int
	{
		switch (hitgroup)
		{
		case hitgroup_head:
			return hitbox_head;
		case hitgroup_chest:
			return hitbox_chest;
		case hitgroup_stomach:
			return hitbox_stomach;
		case hitgroup_leftarm:
			return hitbox_left_hand;
		case hitgroup_rightarm:
			return hitbox_right_hand;
		case hitgroup_leftleg:
			return hitbox_right_calf;
		case hitgroup_rightleg:
			return hitbox_left_calf;
		default:
			return hitbox_pelvis;
		}
	};

	auto get_hitbox_name_from_hitgroup = [get_hitbox_by_hitgroup](const int hitgroup) -> std::string
	{
		switch (get_hitbox_by_hitgroup(hitgroup))
		{
		case hitbox_head:
			return _("head");
		case hitbox_chest:
			return _("chest");
		case hitbox_stomach:
			return _("stomach");
		case hitbox_pelvis:
			return _("pelvis");
		case hitbox_right_upper_arm:
		case hitbox_right_forearm:
		case hitbox_right_hand:
			return _("left arm");
		case hitbox_left_upper_arm:
		case hitbox_left_forearm:
		case hitbox_left_hand:
			return _("right arm");
		case hitbox_right_thigh:
		case hitbox_right_calf:
			return _("left leg");
		case hitbox_left_thigh:
		case hitbox_left_calf:
			return _("right leg");
		case hitbox_right_foot:
			return _("left foot");
		case hitbox_left_foot:
			return _("right foot");
		default:
			return _("generic");
		}
	};

	if (attacker == interfaces::m_engine->get_local_player())
	{
		player_info_t info{};
		interfaces::m_engine->get_player_info(hurt_player->get_index(), &info);

		std::string hit_str = "hit in ";
		{
			hit_str += info.m_name;
			hit_str += " for ";
			hit_str += std::to_string(event->get_int(_("dmg_health")));
			hit_str += " in ";
			hit_str += get_hitbox_name_from_hitgroup(event->get_int(_("hitgroup")));
		}

		logs::push_log(hit_str);
	}
}

void c_events::player_death(i_game_event* event, const char* event_name) const
{
	if (strcmp(event_name, _("player_death")))
		return;
}
