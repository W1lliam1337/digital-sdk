#pragma once

class i_global_vars_base {
public:
	float		m_realtime{};						// 0x0000
	int			m_frame_count{};					// 0x0004
	float		m_abs_frame_time{};					// 0x0008
	float		m_abs_frame_start_time_std_dev{};	// 0x000C
	float		m_current_time{};                   // 0x0010
	float		m_frame_time{};						// 0x0014
	int			m_max_clients{};					// 0x0018
	int			m_tick_count{};						// 0x001C
	float		m_interval_per_tick{};				// 0x0020
	float		m_interpolation_amount{};			// 0x0024
	int			m_sim_ticks_this_frame{};			// 0x0028
	int			m_network_protocol{};				// 0x002C
	void* m_save_data{};						// 0x0030
	bool		m_client{};							// 0x0031
	bool		m_remote_client{};					// 0x0032
	int			m_timestamp_networking_base{};
	int			m_timestamp_randomize_window{};
};