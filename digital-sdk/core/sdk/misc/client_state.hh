#ifndef VALVE_CLIENT_STATE
#define VALVE_CLIENT_STATE

#include "../../utils/utils.hh"
#include "client_class.hh"
#include <cstdint>

struct bf_write;
class bf_read;

class c_event_info {
public:
    int m_class_id{ };
    float m_delay{ };
private:
    [[maybe_unused]] std::byte pad1[ 4 ]{ };
public:
    c_client_class* m_client_class{ };
private:
    [[maybe_unused]] std::byte pad2[ 40 ]{ };
public:
    c_event_info* m_next{ };
};

class i_net_message {
public:
    virtual ~i_net_message( ){ };
    virtual void set_net_channel( void* netchan ) = 0;
    virtual void set_reliable( bool state ) = 0;
    virtual bool process( void ) = 0;
    virtual bool read_from_buffer( bf_read& buffer ) = 0;
    virtual bool write_to_buffer( bf_write& buffer ) = 0;
    virtual bool is_reliable( void ) const = 0;
    virtual int get_type( void ) const = 0;
    virtual int get_group( void ) const = 0;
    virtual const char* get_name( void ) const = 0;
    virtual void* get_net_channel( void ) const = 0;
    virtual const char* to_string( void ) const = 0;
};

class i_net_channel {
public:
    virtual ~i_net_channel( ) = default;
    virtual const char* get_name( ) const = 0;
    virtual const char* get_address( ) const = 0;
    virtual float get_time( ) const = 0;
    virtual float get_time_connected( ) const = 0;
    virtual int get_buffer_size( ) const = 0;
    virtual int get_data_rate( ) const = 0;
    virtual bool is_loopback( ) const = 0;
    virtual bool is_timing_out( ) const = 0;
    virtual bool is_playback( ) const = 0;
    virtual float get_latency( int flow ) const = 0;
    virtual float get_avg_latency( int flow ) const = 0;
};

class c_net_channel {
    [[maybe_unused]] std::byte pad0[ 0x18 ]{ };
public:
    int m_out_sequence_nr{ };
    int m_in_sequence_nr{ };
    int m_out_sequence_nr_ack{ };
    int m_out_reliable_state{ };
    int m_in_reliable_state{ };
    int m_choked_packets{ };

    VFUNC( int( __thiscall* )( void*, int ), send_datagram( const int a2 ), 46, a2 );
    VFUNC( bool( __thiscall* )( void*, i_net_message*, bool, bool ),
           send_net_msg( i_net_message* msg, const bool force_reliable, const bool voice = false ), 40, msg,
           force_reliable, voice );
};

class c_clock_drift_mgr {
public:
    float m_clock_offsets[ 16 ]{ };
    std::uint32_t m_cur_clock_offset{ };
    std::uint32_t m_server_tick{ };
    std::uint32_t m_client_tick{ };
};

class c_client_state {
    [[maybe_unused]] std::byte pad0[ 0x9C ]{ };
public:
    c_net_channel* m_net_channel{ };
    int m_challenge_number{ };
private:
    [[maybe_unused]] std::byte pad1[ 4 ]{ };
public:
    double m_connect_time{ };
    int m_retry_number{ };
private:
    [[maybe_unused]] std::byte pad2[ 0x54 ]{ };
public:
    int m_signon_state{ };
private:
    [[maybe_unused]] std::byte pad3[ 4 ]{ };
public:
    double m_next_cmd_time{ };
    int m_server_count{ };
    int m_cur_sequence{ };
private:
    [[maybe_unused]] std::byte pad4[ 8 ]{ };
public:
    struct {
        float m_clock_offsets[ 0x10 ]{ };
        int m_cur_clock_offset{ };
        int m_server_tick{ };
        int m_client_tick{ };
    } m_clock_drift_mgr{ };
    int m_delta_tick{ };
private:
    [[maybe_unused]] std::byte pad5[ 4 ]{ };
public:
    int m_view_entity{ };
    int m_player_slot{ };
    bool m_paused{ };
private:
    [[maybe_unused]] std::byte pad6[ 3 ]{ };
    char m_level_name[ 0x104 ]{ };
    char m_level_name_short[ 0x28 ]{ };
    [[maybe_unused]] std::byte pad7[ 0xD4 ]{ };
public:
    int m_max_clients{ };
private:
    [[maybe_unused]] std::byte pad8[ 0x4994 ]{ };
public:
    int m_old_tick_count{ };
    float m_tick_remainder{ };
    float m_frame_time{ };
    int m_last_outgoing_command{ };
    int m_choked_cmds{ };
    int m_last_command_ack{ };
    int m_last_server_tick{ };
    int m_command_ack{ };
    int m_sound_sequence{ };
    int m_last_progress_percent{ };
    bool m_is_hltv{ };
private:
    [[maybe_unused]] std::byte pad9[ 0x4B ]{ };
public:
    vec3_t m_view_angles{ };
private:
    [[maybe_unused]] std::byte pad10[ 0xCC ]{ };
public:
    c_event_info* m_events{ };
};

#endif