#pragma once
#include "../sdk/sdk.hpp"

struct key_bind_t
{
	int m_mode_selected = 0;
	int m_key_selected = 0;
};

class c_utils : public c_singleton<c_utils>
{
public:
	static uint8_t* find_sig(HMODULE module, const std::string& byte_array);
	bool is_bind_active(key_bind_t key_bind);
	static void init_key_sys(UINT msg, WPARAM wParam);
	void fix_movement() const;
	void fix_process_packets();
};

