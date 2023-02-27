#ifndef VALVE_DT_RECV
#define VALVE_DT_RECV

struct recv_table_t;
class c_recv_proxy_data;
struct recv_prop_t;

using array_length_recv_proxy_fn = void( __cdecl* )( void*, int, int );
using recv_var_proxy_fn = void ( * )( const c_recv_proxy_data*, void*, void* );
using data_table_recv_var_proxy_fn = void( __cdecl* )( recv_prop_t*, void**, void*, int );

enum e_send_prop_type { dpt_int = 0, dpt_float, dpt_vector, dpt_string, dpt_array, dpt_datatable };

struct d_variant_t {
    union {
        float m_float;
        long m_int;
        char* m_string;
        void* m_data;
        float m_vector[ 3 ];
        long long m_int_64;
    };

    e_send_prop_type m_type;
};

struct recv_prop_t {
    char* m_var_name{ };
    int m_recv_type{ };
    int m_flags{ };
    int m_string_buffer_size{ };
    bool m_inside_array{ };
    const void* m_extra_data{ };
    recv_prop_t* m_array_prop{ };
    array_length_recv_proxy_fn m_array_length_proxy{ };
    recv_var_proxy_fn m_proxy_function{ };
    data_table_recv_var_proxy_fn m_data_table_proxy_function{ };
    recv_table_t* m_data_table{ };
    int m_offset{ };
    int m_element_stride{ };
    int m_elements{ };
    const char* m_parent_array_prop_name{ };
};

struct recv_table_t {
    recv_prop_t* m_props_pointer{ };
    int m_props{ };
    void* m_decoder{ };
    char* m_net_table_name{ };
    bool m_initialized{ };
    bool m_in_main_list{ };
};

class c_recv_proxy_data {
public:
    recv_prop_t* m_recv_prop{ };
    d_variant_t m_value{ };
    int m_element{ };
    int m_object_id{ };
};

#endif