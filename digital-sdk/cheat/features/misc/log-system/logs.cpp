#include "logs.h"

void logs::push_log(const std::string& text, const std::string& icon, const c_color color) noexcept
{
	m_logs.emplace_front
	(
		logs_data_t
		(
			text,					// m_text
			icon,					// m_icon
			interfaces::m_globals->m_realtime,	// m_creation_time
			color					// m_color
		)
	);

	while (m_logs.size() > 10)
		m_logs.pop_back();
}

void logs::instance()
{
	for (int i{}; i < m_logs.size(); i++)
	{
		auto* logs = &m_logs[i];
		if (!logs)
			continue;

		const auto time_after_creation = interfaces::m_globals->m_realtime - logs->m_creation_time;
		if (time_after_creation >= 5.0f)
		{
			m_logs.erase(m_logs.begin() + i);
			/* m_logs.clear(); */
		}

		if (time_after_creation < 4.75f)
		{
			if (time_after_creation <= 0.25f)
				logs->m_spacing = std::clamp((1.0f - (0.25f - time_after_creation) / 0.25f) * 24.0f, 0.0f, 24.0f);
		}
		else { logs->m_spacing = std::clamp((5.0f - time_after_creation) / 0.25f * 24.0f, 0.0f, 24.0f); }

		auto spacing = 0.0f;
		if (i)
		{
			for (int i2{}; i2 < i; i2++)
				spacing += m_logs[i2].m_spacing;
		}

		if (!logs->m_printed)
		{
			interfaces::m_cvar->console_color_print_f(c_color(49, 106, 218), _("%s\n"), logs->m_text);
			logs->m_printed = true;
		}

		render::text(render::fonts::m_logs, ImVec2(5.0f, spacing + 2.5f), logs->m_text, logs->m_color, false);
	}
}