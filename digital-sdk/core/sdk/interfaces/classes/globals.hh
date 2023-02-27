#ifndef VALVE_GLOBALS
#define VALVE_GLOBALS

class i_global_vars {
public:
    float m_realtime{ };
    int m_frame_count{ };
    float m_abs_frame_time{ };
    float m_abs_frame_start_time{ };
    float m_current_time{ };
    float m_frame_time{ };
    int m_max_clients{ };
    int m_tick_count{ };
    float m_interval_per_tick{ };
    float m_interpolation_amount{ };
    int m_sim_ticks_this_frame{ };
    int m_network_protocol{ };
    void* m_save_data{ };
    bool m_client{ };
    bool m_remote_client{ };
    int m_time_stamp_networking_base{ };
    int m_time_stamp_randomize_window{ };
};

#endif