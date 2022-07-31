#pragma once
#include "game/player.h"
#include "game/weapon.h"
#include "game/inferno.h"
#include "interfaces/interfaces.h"

namespace ctx
{
	namespace packet_data
	{
		inline c_user_cmd* m_cmd{};
		inline bool m_send_packet{};
	}

	namespace menu_data
	{
		inline bool m_is_menu_opened{};
	}

	namespace local_data
	{
		inline c_vec3 m_shoot_pos{};
	}

	static __forceinline c_base_player* local()
	{
		if (!interfaces::m_entity_list || !interfaces::m_engine)
			return nullptr;

		const auto local_player = reinterpret_cast<c_base_player*>(interfaces::m_entity_list->get_client_entity(interfaces::m_engine->get_local_player()));
		if (!local_player)
			return nullptr;

		return local_player;
	}

	static int time_to_ticks(const float time)
	{
		return static_cast<int>(0.5f + time / interfaces::m_globals->m_interval_per_tick);
	}

	static float ticks_to_time(const int tick)
	{
		return static_cast<float>(tick) * interfaces::m_globals->m_interval_per_tick;
	}
}