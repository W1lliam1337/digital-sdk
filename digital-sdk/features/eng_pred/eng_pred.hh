#ifndef ENG_PRED_H
#define ENG_PRED_H

#include "../../context.hh"

class c_eng_pred final {
    struct backup_data_t {
        backup_data_t( ) = default;

        void store( ) {
            m_tick_count = g_interfaces->m_globals->m_tick_count;
            m_current_time = g_interfaces->m_globals->m_current_time;
            m_frame_time = g_interfaces->m_globals->m_frame_time;
            m_in_prediction = g_interfaces->m_prediction->m_in_prediction;
            m_first_time_predicted = g_interfaces->m_prediction->m_first_time_predicted;
        }

        void reset( ) const {
            g_interfaces->m_globals->m_tick_count = m_tick_count;
            g_interfaces->m_globals->m_current_time = m_current_time;
            g_interfaces->m_globals->m_frame_time = m_frame_time;
            g_interfaces->m_prediction->m_in_prediction = m_in_prediction;
            g_interfaces->m_prediction->m_first_time_predicted = m_first_time_predicted;
        }

        int m_tick_count{ };

        float m_current_time{ }, m_frame_time{ };

        bool m_in_prediction{ }, m_first_time_predicted{ };
    } m_backup_data{ };
public:
    void predict( const std::function< void( ) >& fn );
    void begin( );
    void end( ) const;
    static void update( );
};

inline const std::unique_ptr< c_eng_pred > g_eng_pred{ new c_eng_pred( ) };

#endif