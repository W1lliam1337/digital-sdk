#pragma once

struct model_t {
	char			pad0[4];
	char			m_name[260];
	int				m_load_flags;
	int				m_server_count;
	int				m_type;
	int				m_flags;
	vec3_t			m_mins;
	vec3_t			m_maxs;
	float			m_radius;
	void* m_key_values;
	char			pad1[24];
};

