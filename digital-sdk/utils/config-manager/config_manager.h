#pragma once
#include "../utils.h"
namespace cfg
{
	namespace legit
	{
		inline bool m_enabled{};
		inline bool m_auto_fire{};
		inline bool m_auto_scope{};
		inline bool m_silent{};

		inline int m_fov{};
		inline int m_smooth{ 1 };
		inline int m_rcs_x{};
		inline int m_rcs_y{};
	}

	namespace misc
	{
		inline bool m_bunny_hop{};
		inline bool m_no_scope{};
		inline bool m_infinity_duck{};
		inline bool m_remove_molotov{};

		inline int m_third_person_distance{};
		inline key_bind_t m_third_person_bind = key_bind_t();
	}

	namespace anti_aim
	{
		inline bool m_enabled{};
		inline bool m_desync{};
		inline bool m_fake_lags{};

		inline int m_desync_right_range{ 58 };
		inline int m_desync_left_range{ 58 };

		inline int m_fake_lags_factor{};
		inline int m_pitch_mode{};
		inline int m_desync_mode{};

		inline key_bind_t m_inverter = key_bind_t();
	}

	namespace esp
	{
		struct player_t
		{
			bool m_name{};
			bool m_box{};
			bool m_health_bar{};
			bool m_health_text{};
		}inline m_player[4]{};
	}
}