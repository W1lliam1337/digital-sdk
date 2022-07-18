#pragma once
#include "game/player.h"
#include "game/weapon.h"
#include "game/inferno.h"
#include "interfaces/interfaces.h"

class c_hitbox_data
{
public:
	int m_hitbox{};
	vec3_t m_point{};
};

class c_sdk
{
public:
	struct accuracy_data_t
	{
		std::array<float, 255> m_inaccuracy;
		std::array<float, 255> m_spread;
		std::array<float, 255> m_first_random;
		std::array<float, 255> m_second_random;
		bool m_has_valid_accuracy_data = false;
	}m_accuracy_data{};
	struct c_packet_data
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
		vec3_t m_shoot_pos{};
	}m_local_data{};

	struct rage_data_t
	{
		c_base_player* m_target{};
		c_hitbox_data m_hitbox_data{};
	}m_rage_data{};

	static __forceinline c_base_player* m_local()
	{
		if (!g_interfaces->m_entity_list || !g_interfaces->m_engine)
			return nullptr;

		const auto local_player = reinterpret_cast<c_base_player*>(g_interfaces->m_entity_list->get_client_entity(g_interfaces->m_engine->get_local_player()));
		if (!local_player)
			return nullptr;

		return local_player;
	}
};

#define TIME_TO_TICKS(time_) ((int)(0.5f + (float)((time_)) / g_interfaces->m_globals->m_interval_per_tick))
#define TICKS_TO_TIME(tick) (float)((tick) * g_interfaces->m_globals->m_interval_per_tick)

inline const auto g_sdk = std::make_unique<c_sdk>();