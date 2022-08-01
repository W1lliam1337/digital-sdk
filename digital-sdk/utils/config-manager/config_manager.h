#pragma once
#include "../utils.h"

class c_cfg
{
public:
	struct legit_t
	{
		bool m_enabled{};
		bool m_auto_fire{};
		bool m_auto_scope{};
		bool m_silent{};

		int m_fov{};
		int m_smooth{ 1 };
		int m_rcs_x{};
		int m_rcs_y{};
	}m_legit{};

	struct misc_t
	{
		bool m_bunny_hop{};
		bool m_no_scope{};
		bool m_infinity_duck{};
		bool m_remove_molotov{};
		int m_fov{90};
		int m_third_person_distance{};
		key_bind_t m_third_person_bind = key_bind_t();
	}m_misc{};

	struct anti_aim_t
	{
		bool m_enabled{};
		bool m_desync{};
		bool m_fake_lags{};

		int m_desync_right_range{ 58 };
		int m_desync_left_range{ 58 };

		int m_fake_lags_factor{};
		int m_pitch_mode{};
		int m_desync_mode{};

		key_bind_t m_inverter = key_bind_t();
	}m_anti_aim{};

	struct esp_t
	{
		struct player_t
		{
			bool m_name{};
			bool m_box{};
			bool m_health_bar{};
			bool m_health_text{};
			bool m_chams{};
			bool m_invisible_chams{};
		}m_player[4]{};
	}m_esp{};
};

inline auto g_cfg = c_cfg();