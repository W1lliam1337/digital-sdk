#pragma once
#include "../../../../sdk/sdk.h"

struct logs_data_t
{
	logs_data_t(const std::string& text, const std::string& icon, const float time, const c_color color)
	{
		this->m_text = text;
		this->m_icon = icon;
		this->m_color = color;
		this->m_creation_time = time;
	}

	std::string m_text{};
	std::string m_icon{};
	float m_creation_time{};
	float m_spacing{};
	bool m_printed{};
	c_color m_color{};
};

namespace logs
{
	inline std::deque <logs_data_t> m_logs{};
	void instance();
	void push_log(const std::string& text, const std::string& icon = "", c_color color = { 255, 255 , 255 }) noexcept;
}