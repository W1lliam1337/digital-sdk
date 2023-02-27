#ifndef VALVE_TIER1_KEY_VALUES
#define VALVE_TIER1_KEY_VALUES

#include <cstdint>

using symbol_fn = bool( __cdecl* )( const char* );
class c_key_values {
public:
    void load_from_buffer( const char* resource_name, const char* buf, void* file_sys = nullptr,
                           const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr );
    [[nodiscard]] const char* get_name( );
    c_key_values( const char* key_name, void* unk = nullptr, void* unk2 = nullptr );
private:
    std::uint32_t m_key_name : 24;
    std::uint32_t m_key_name_case_sensitive1 : 8;
    char* m_sz_value{ };
    wchar_t* m_wsz_value{ };

    union {
        int m_i_value;
        float m_f_value;
        void* m_value;
        unsigned char m_color[ 4 ];
    };

    std::int8_t m_data_type{ };
    bool m_has_escape_sequences{ };
    std::uint16_t m_key_name_case_sensitive2{ };
    void* m_unk{ };
    bool m_has_case_insensitive_key_symbol{ };
    c_key_values* m_peer{ };
    c_key_values* m_sub{ };
    c_key_values* m_chain{ };
    symbol_fn m_expression_get_symbol_proc{ };
};

#endif