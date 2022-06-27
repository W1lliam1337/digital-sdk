#pragma once

struct key_bind_t
{
	int m_mode_selected = 0;
	int m_key_selected = 0;
};

//#include "../singleton/singleton.h"
//#include "../json/json.h"
//#include "base64.h"
//
//struct key_bind_t;
//
//class c_config_manager : public c_singleton<c_config_manager>
//{
//public:
//	class c_item
//	{
//	public:
//		std::string name;
//		void* pointer;
//		std::string type;
//
//		c_item(const std::string name, void* pointer, const std::string& type) 
//		{
//			this->name = name;
//			this->pointer = pointer;
//			this->type = type;
//		}
//	};
//
//	std::vector <c_item*> m_items;
//	std::vector<std::string> m_files;
//	int m_selected_cfg;
//
//	void test();
//	void push_item(float* pointer, float value, const std::string& name);
//	void push_item(int* pointer, int value, const std::string& name);
//	void push_item(bool* pointer, bool value, const std::string& name);
//	void push_item(key_bind_t* pointer, key_bind_t value, const std::string& name);
//	void save_config(const std::string& config) const;
//};

struct config_t
{
	struct legit_bot_t
	{
		bool m_enabled{};
		bool m_silent{};
		bool m_auto_fire{};
		bool m_auto_scope{};
		bool m_rcs{};

		int m_smooth{1};
		int m_fov{1};
		int m_rcs_x{};
		int m_rcs_y{};
	}m_legit_bot{};
	struct esp_t
	{
		bool m_enable{};
		bool m_name{};
	}m_esp{};

	struct misc_t
	{
		bool m_third_person{};
		bool m_no_scope{};
		bool m_bunny_hop{};

		int m_third_person_distance{};
		key_bind_t m_third_person_bind;
	}m_misc{};

	struct anti_aim_t
	{
		bool m_enable{};
		bool m_desync{};
		bool m_fake_lags{};

		int m_desync_right_range = 58;
		int m_desync_left_range = 58;

		int m_fake_lags_factor{};
		int m_pitch_mode{};
		int m_desync_mode{};

		//key_bind_t m_inverter;
	}m_anti_aim;
};

inline config_t g_cfg = config_t();