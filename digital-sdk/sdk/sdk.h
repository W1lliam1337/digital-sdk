#pragma once
#include "game/player.h"
#include "game/weapon.h"
#include "game/inferno.h"
#include "interfaces/interfaces.h"

class c_sdk
{
public:
	struct packet_data_t
	{
		c_user_cmd* m_cmd{};
		bool m_send_packet{};
	}m_packet_data{};

	struct menu_data_t
	{
		bool m_is_menu_opened{};
	}m_menu_data{};

	struct local_data_t
	{
		c_vec3 m_shoot_pos{};
	}m_local_data{};

	static __forceinline c_base_player* m_local()
	{
		if (!g_interfaces->m_entity_list || !g_interfaces->m_engine)
			return nullptr;

		const auto local_player = reinterpret_cast<c_base_player*>(g_interfaces->m_entity_list->get_client_entity(g_interfaces->m_engine->get_local_player()));
		if (!local_player)
			return nullptr;

		return local_player;
	}

	static int time_to_ticks(const float time)
	{
		return static_cast<int>(0.5f + time / g_interfaces->m_globals->m_interval_per_tick);
	}

	static float ticks_to_time(const int tick)
	{
		return static_cast<float>(tick) * g_interfaces->m_globals->m_interval_per_tick;
	}
};

inline const auto g_sdk = std::make_unique<c_sdk>();