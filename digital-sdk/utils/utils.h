#pragma once
#include <cstdint>
#include <string>
#include <wtypes.h>
#include <array>
#include <cstddef>
#include <memory>
#include <vector>

struct key_bind_t
{
	int m_mode_selected = 0;
	int m_key_selected = 0;
};

class c_utils
{
public:
	struct m_key_data_t
	{
		std::array<bool, 256> m_holded_keys{};
		std::array<bool, 256> m_toggled_keys{};
	}m_key_data;

	static uint8_t* find_sig(HMODULE module, const std::string& byte_array);
	template <typename T>
	static __forceinline T call_vfunc(void* instance, std::size_t index)
	{
		return (*static_cast<T**>(instance))[index];
	}

	[[nodiscard]] bool is_bind_active(key_bind_t key_bind) const noexcept;
	void init_key_sys(UINT msg, WPARAM wParam);
};

inline const auto g_utils = std::make_unique<c_utils>();