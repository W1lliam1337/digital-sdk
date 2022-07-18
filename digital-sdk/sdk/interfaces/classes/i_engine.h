#pragma once
#include <functional>
#include "../../../utils/utils.h"

class VMatrix;

struct player_info_t
{
	int64_t m_something;

	union
	{
		int64_t m_steam_id64; //0x0008 - SteamID64
		struct
		{
			int32_t m_xuid_low;
			int32_t m_xuid_high;
		};
	};

	char m_name[128];
	int m_user_id;
	char m_steam_id[20];
	char pad_0x00A8[0x10];
	unsigned long m_un_steam_id;
	char m_friends_name[128];
	bool m_is_bot;
	bool m_is_hltv;
	unsigned int m_custom_files[4];
	unsigned char m_files_downloaded;
};

class i_engine
{
public:
	bool get_player_info(const int ent_num, player_info_t* info)
	{
		return g_utils->call_vfunc< bool(__thiscall*)(void*, int, player_info_t*) >(this, 8)(this, ent_num, info);
	}

	int get_player_for_user_id(const int user_id)
	{
		return g_utils->call_vfunc<int(__thiscall*)(void*, int)>(this, 9)(this, user_id);
	}

	bool con_is_visible()
	{
		return g_utils->call_vfunc< bool(__thiscall*)(void*) >(this, 11)(this);
	}

	int get_local_player()
	{
		return g_utils->call_vfunc< int(__thiscall*)(void*) >(this, 12)(this);
	}

	void get_view_angles(qangle_t& angle)
	{
		return g_utils->call_vfunc< void(__thiscall*)(void*, qangle_t&) >(this, 18)(this, angle);
	}

	void set_view_angles(qangle_t& angle)
	{
		return g_utils->call_vfunc< void(__thiscall*)(void*, qangle_t&) >(this, 19)(this, angle);
	}

	bool is_in_game()
	{
		return g_utils->call_vfunc< bool(__thiscall*)(void*) >(this, 26)(this);
	}

	bool is_connected()
	{
		return g_utils->call_vfunc< bool(__thiscall*)(void*) >(this, 27)(this);
	}

	uintptr_t* get_bsp_tree_query()
	{
		return g_utils->call_vfunc< uintptr_t* (__thiscall*)(void*) >(this, 43)(this);
	}

	bool is_voice_recording()
	{
		return g_utils->call_vfunc< bool(__thiscall*)(void*) >(this, 224)(this);
	}

	void execute_client_cmd(const char* cmd_string)
	{
		return g_utils->call_vfunc< void(__thiscall*)(void*, const char*) >(this, 108)(this, cmd_string);
	}

	const VMatrix& world_to_screen_matrix() {
		return g_utils->call_vfunc<const VMatrix& (__thiscall*)(void*)>(this, 37)(this);
	}

	void get_screen_size(int& width, int& height)
	{
		return g_utils->call_vfunc<void(__thiscall*)(void*, int&, int&)>(this, 44)(this, std::ref(width), std::ref(height));  //width, height
	}
};

