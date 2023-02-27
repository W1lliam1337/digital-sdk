#ifndef VALVE_DATA_MAP
#define VALVE_DATA_MAP

enum e_field_types : int {
    field_void = 0,
    field_float,
    field_string,
    field_vector,
    field_quaternion,
    field_integer,
    field_boolean,
    field_short,
    field_character,
    field_color32,
    field_embedded,
    field_custom,
    field_classptr,
    field_ehandle,
    field_edict,
    field_position_vector,
    field_time,
    field_tick,
    field_modelname,
    field_soundname,
    field_input,
    field_function,
    field_vmatrix,
    field_vmatrix_worldspace,
    field_matrix3x4_worldspace,
    field_interval,
    field_modelindex,
    field_materialindex,
    field_vector2d,
    field_integer64,
    field_vector4d,
    field_typecount
};

struct data_map_t;

struct type_description_t {
    e_field_types m_field_type{ };
    const char* m_field_name{ };
    int m_field_offset[ 2 ]{ };
    unsigned short m_field_size{ };
    short m_flags{ };
private:
    [[maybe_unused]] std::byte pad0[ 0xC ]{ };
public:
    data_map_t* m_type_description{ };
private:
    [[maybe_unused]] std::byte pad1[ 0x18 ]{ };
};

struct data_map_t {
    type_description_t* m_data_desc{ };
    int m_data_num_fields{ };
    const char* m_data_class_name{ };
    data_map_t* m_base_map{ };
    int m_packed_size{ };
    void* m_optimized_data_map{ };
};

#endif