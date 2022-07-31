#include "utils.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/sdk.h"

uint8_t* utils::sig(const HMODULE module, const std::string& byte_array)
{
	if (!module)
		return nullptr;

	static auto pattern_to_byte = [](const char* pattern)
	{
		auto bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + std::strlen(pattern);

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
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
	const auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte(byte_array.c_str());
	const auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	const auto pattern_size = pattern_bytes.size();
	const auto pattern_data = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - pattern_size; ++i)
	{
		bool found = true;

		for (auto j = 0ul; j < pattern_size; ++j)
		{
			if (scan_bytes[i + j] != pattern_data[j] && pattern_data[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found)
			return &scan_bytes[i];
	}

	return nullptr;
}

bool utils::is_bind_active(const key_bind_t key_bind) noexcept
{
	if (key_bind.m_key_selected <= 0)
		return false;

	switch (key_bind.m_mode_selected)
	{
	case 0:
		return m_key_data.m_toggled_keys[key_bind.m_key_selected];
	case 1:
		return m_key_data.m_holded_keys[key_bind.m_key_selected];
	default: break;
	}

	return false;
}

void utils::init_key_sys(const UINT msg, const WPARAM wParam)
{
	bool is_handle_possible = true;
	if (interfaces::m_engine->is_connected() && interfaces::m_engine->is_in_game())
	{
		if (interfaces::m_engine->con_is_visible())
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
				m_key_data.m_toggled_keys[VK_MENU] = !m_key_data.m_toggled_keys[VK_MENU];
				m_key_data.m_holded_keys[VK_MENU] = true;
			}
			else if (msg == WM_SYSKEYUP)
				m_key_data.m_holded_keys[VK_MENU] = false;
		}

		switch (msg)
		{
		case WM_LBUTTONDOWN:
			m_key_data.m_toggled_keys[VK_LBUTTON] = !m_key_data.m_toggled_keys[VK_LBUTTON];
			m_key_data.m_holded_keys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			m_key_data.m_holded_keys[VK_LBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				m_key_data.m_holded_keys[VK_XBUTTON1] = true;
				m_key_data.m_toggled_keys[VK_XBUTTON1] = !m_key_data.m_toggled_keys[VK_XBUTTON1];
			}

			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
			{
				m_key_data.m_holded_keys[VK_XBUTTON2] = true;
				m_key_data.m_toggled_keys[VK_XBUTTON2] = !m_key_data.m_toggled_keys[VK_XBUTTON2];
			}
			break;
		case WM_XBUTTONUP:
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
				m_key_data.m_holded_keys[VK_XBUTTON1] = false;

			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
				m_key_data.m_holded_keys[VK_XBUTTON2] = false;
			break;
		case WM_MBUTTONDOWN:
			m_key_data.m_holded_keys[VK_MBUTTON] = true;
			m_key_data.m_toggled_keys[VK_MBUTTON] = !m_key_data.m_toggled_keys[VK_MBUTTON];
			break;
		case WM_MBUTTONUP:
			m_key_data.m_holded_keys[VK_MBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			m_key_data.m_toggled_keys[VK_RBUTTON] = !m_key_data.m_toggled_keys[VK_RBUTTON];
			m_key_data.m_holded_keys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			m_key_data.m_holded_keys[VK_RBUTTON] = false;
			break;
		case WM_KEYDOWN:
			m_key_data.m_toggled_keys[wParam] = !m_key_data.m_toggled_keys[wParam];
			m_key_data.m_holded_keys[wParam] = true;
			break;
		case WM_KEYUP:
			m_key_data.m_holded_keys[wParam] = false;
			break;
		default: break;
		}
	}

	if (!ctx::local())
	{
		for (int i = 0; i < 256; i++)
		{
			if (i == VK_INSERT)
				continue;

			m_key_data.m_holded_keys[i] = false;
			m_key_data.m_toggled_keys[i] = false;
		}
	}
}