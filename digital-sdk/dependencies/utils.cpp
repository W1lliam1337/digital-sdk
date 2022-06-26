#include "utils.h"
#include "../sdk/sdk.hpp"

uint8_t* c_utils::find_sig(const HMODULE module, const std::string& byte_array)
{
	static auto pattern_to_byte = [](const char* pattern)
	{
		auto bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dos_header->
		e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte(byte_array.c_str());
	const auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	const auto s = pattern_bytes.size();
	const auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scan_bytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return &scan_bytes[i];
		}
	}
	return nullptr;
}

bool c_utils::is_bind_active(const key_bind_t key_bind)
{
	if (key_bind.m_key_selected <= 0)
	{
		if (key_bind.m_mode_selected == 3)
			return true;

		return false;
	}

	switch (key_bind.m_mode_selected)
	{
	case 0:
		return g_sdk.m_key_data.m_toggled_keys[key_bind.m_key_selected];
	case 1:
		return g_sdk.m_key_data.m_holded_keys[key_bind.m_key_selected];
	case 2:
		return !g_sdk.m_key_data.m_holded_keys[key_bind.m_key_selected];
	case 3:
		return true;
	default: break;
	}

	return false;
}

void c_utils::init_key_sys(const UINT msg, const WPARAM wParam)
{
	bool is_handle_possible = true;
	if (g_sdk.m_interfaces.m_engine->is_connected() && g_sdk.m_interfaces.m_engine->is_in_game())
	{
		if (g_sdk.m_interfaces.m_engine->con_is_visible())
		{
			is_handle_possible = false;
			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP || msg ==
				WM_XBUTTONDOWN || msg == WM_XBUTTONUP || msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
				is_handle_possible = true;

			if (msg == WM_KEYDOWN || msg == WM_KEYUP)
				if (wParam == VK_INSERT)
					is_handle_possible = true;
		}
	}

	if (is_handle_possible)
	{
		if (wParam == VK_MENU)
		{
			if (msg == WM_SYSKEYDOWN)
			{
				g_sdk.m_key_data.m_toggled_keys[VK_MENU] = !g_sdk.m_key_data.m_toggled_keys[VK_MENU];
				g_sdk.m_key_data.m_holded_keys[VK_MENU] = true;
			}
			else if (msg == WM_SYSKEYUP)
				g_sdk.m_key_data.m_holded_keys[VK_MENU] = false;
		}

		switch (msg)
		{
		case WM_LBUTTONDOWN:
			g_sdk.m_key_data.m_toggled_keys[VK_LBUTTON] = !g_sdk.m_key_data.m_toggled_keys[VK_LBUTTON];
			g_sdk.m_key_data.m_holded_keys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			g_sdk.m_key_data.m_holded_keys[VK_LBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				g_sdk.m_key_data.m_holded_keys[VK_XBUTTON1] = true;
				g_sdk.m_key_data.m_toggled_keys[VK_XBUTTON1] = !g_sdk.m_key_data.m_toggled_keys[VK_XBUTTON1];
			}

			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
			{
				g_sdk.m_key_data.m_holded_keys[VK_XBUTTON2] = true;
				g_sdk.m_key_data.m_toggled_keys[VK_XBUTTON2] = !g_sdk.m_key_data.m_toggled_keys[VK_XBUTTON2];
			}
			break;
		case WM_XBUTTONUP:
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				g_sdk.m_key_data.m_holded_keys[VK_XBUTTON1] = false;

			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
				g_sdk.m_key_data.m_holded_keys[VK_XBUTTON2] = false;
			break;
		case WM_MBUTTONDOWN:
			g_sdk.m_key_data.m_holded_keys[VK_MBUTTON] = true;
			g_sdk.m_key_data.m_toggled_keys[VK_MBUTTON] = !g_sdk.m_key_data.m_toggled_keys[VK_MBUTTON];
			break;
		case WM_MBUTTONUP:
			g_sdk.m_key_data.m_holded_keys[VK_MBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			g_sdk.m_key_data.m_toggled_keys[VK_RBUTTON] = !g_sdk.m_key_data.m_toggled_keys[VK_RBUTTON];
			g_sdk.m_key_data.m_holded_keys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			g_sdk.m_key_data.m_holded_keys[VK_RBUTTON] = false;
			break;
		case WM_KEYDOWN:
			g_sdk.m_key_data.m_toggled_keys[wParam] = !g_sdk.m_key_data.m_toggled_keys[wParam];
			g_sdk.m_key_data.m_holded_keys[wParam] = true;
			break;
		case WM_KEYUP:
			g_sdk.m_key_data.m_holded_keys[wParam] = false;
			break;
		default: break;
		}
	}

	if (!g_sdk.m_local())
	{
		for (int i = 0; i < 256; i++)
		{
			if (i == VK_INSERT)
				continue;

			g_sdk.m_key_data.m_holded_keys[i] = false;
			g_sdk.m_key_data.m_toggled_keys[i] = false;
		}
	}
}

void c_utils::fix_movement() const
{
	vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	const auto ang_view_angles = g_sdk.m_packet_data.m_cmd->m_view_angles;
	ang_view_angles.normalize();

	qangle_t wish;
	g_sdk.m_interfaces.m_engine->get_view_angles(wish);

	c_math::get()->angle_vectors(wish, &view_fwd, &view_right, &view_up);
	c_math::get()->angle_vectors(ang_view_angles, &cmd_fwd, &cmd_right, &cmd_up);

	const float v8 = sqrtf(view_fwd.x * view_fwd.x + view_fwd.y * view_fwd.y);
	const float v10 = sqrtf(view_right.x * view_right.x + view_right.y * view_right.y);
	const float v12 = sqrtf(view_up.z * view_up.z);

	const vec3_t norm_view_fwd(1.f / v8 * view_fwd.x, 1.f / v8 * view_fwd.y, 0.f);
	const vec3_t norm_view_right(1.f / v10 * view_right.x, 1.f / v10 * view_right.y, 0.f);
	const vec3_t norm_view_up(0.f, 0.f, 1.f / v12 * view_up.z);

	const float v14 = sqrtf(cmd_fwd.x * cmd_fwd.x + cmd_fwd.y * cmd_fwd.y);
	const float v16 = sqrtf(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y);
	const float v18 = sqrtf(cmd_up.z * cmd_up.z);

	const vec3_t norm_cmd_fwd(1.f / v14 * cmd_fwd.x, 1.f / v14 * cmd_fwd.y, 0.f);
	const vec3_t norm_cmd_right(1.f / v16 * cmd_right.x, 1.f / v16 * cmd_right.y, 0.f);
	const vec3_t norm_cmd_up(0.f, 0.f, 1.f / v18 * cmd_up.z);

	const float v22 = norm_view_fwd.x * g_sdk.m_packet_data.m_cmd->m_forward_move;
	const float v26 = norm_view_fwd.y * g_sdk.m_packet_data.m_cmd->m_forward_move;
	const float v28 = norm_view_fwd.z * g_sdk.m_packet_data.m_cmd->m_forward_move;
	const float v24 = norm_view_right.x * g_sdk.m_packet_data.m_cmd->m_side_move;
	const float v23 = norm_view_right.y * g_sdk.m_packet_data.m_cmd->m_side_move;
	const float v25 = norm_view_right.z * g_sdk.m_packet_data.m_cmd->m_side_move;
	const float v30 = norm_view_up.x * g_sdk.m_packet_data.m_cmd->m_up_move;
	const float v27 = norm_view_up.z * g_sdk.m_packet_data.m_cmd->m_up_move;
	const float v29 = norm_view_up.y * g_sdk.m_packet_data.m_cmd->m_up_move;

	g_sdk.m_packet_data.m_cmd->m_forward_move = norm_cmd_fwd.x * v24 + norm_cmd_fwd.y * v23 + norm_cmd_fwd.z * v25
		+ (norm_cmd_fwd.x * v22 + norm_cmd_fwd.y * v26 + norm_cmd_fwd.z * v28)
		+ (norm_cmd_fwd.y * v30 + norm_cmd_fwd.x * v29 + norm_cmd_fwd.z * v27);
	g_sdk.m_packet_data.m_cmd->m_side_move = norm_cmd_right.x * v24 + norm_cmd_right.y * v23 + norm_cmd_right.z * v25
		+ (norm_cmd_right.x * v22 + norm_cmd_right.y * v26 + norm_cmd_right.z * v28)
		+ (norm_cmd_right.x * v29 + norm_cmd_right.y * v30 + norm_cmd_right.z * v27);
	g_sdk.m_packet_data.m_cmd->m_up_move = norm_cmd_up.x * v23 + norm_cmd_up.y * v24 + norm_cmd_up.z * v25
		+ (norm_cmd_up.x * v26 + norm_cmd_up.y * v22 + norm_cmd_up.z * v28)
		+ (norm_cmd_up.x * v30 + norm_cmd_up.y * v29 + norm_cmd_up.z * v27);

	g_sdk.m_packet_data.m_cmd->m_forward_move = std::clamp(g_sdk.m_packet_data.m_cmd->m_forward_move, -450.f, 450.f);
	g_sdk.m_packet_data.m_cmd->m_side_move = std::clamp(g_sdk.m_packet_data.m_cmd->m_side_move, -450.f, 450.f);
	g_sdk.m_packet_data.m_cmd->m_up_move = std::clamp(g_sdk.m_packet_data.m_cmd->m_up_move, -320.f, 320.f);
}

void c_utils::fix_process_packets()
{
	
}