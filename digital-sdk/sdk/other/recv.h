#pragma once

struct recv_table_t;
struct recv_prop_t;

enum e_send_prop_type {
	dpt_int = 0,
	dpt_float,
	dpt_vector,
	dpt_string,
	dpt_array,
	dpt_datatable
};

struct d_variant_t {
	union {
		float m_float{};
		long m_int;
		char* m_string;
		void* m_data;
		float m_vector[3];
		long long m_int_64;
	};

	e_send_prop_type m_type;
};

struct recv_proxy_data_t {
	const recv_prop_t* m_recv_prop{};
	d_variant_t m_value{};
	int m_element{};
	int m_object_id{};
};

using array_length_recv_proxy_fn = void( __cdecl* )(void* ptr, int id, int len);
using recv_var_proxy_fn = void( __cdecl* )(recv_proxy_data_t* data, void* struct_ptr, void* out);
using data_table_recv_var_proxy_fn = void( __cdecl* )(recv_prop_t* prop, void** out, void* data, int id);

struct recv_prop_t {
	char* m_var_name{};
	int m_recv_type{};
	int m_flags{};
	int m_string_buffer_size{};
	bool m_inside_array{};
	const void* m_extra_data{};
	recv_prop_t* m_array_prop{};
	array_length_recv_proxy_fn m_array_length_proxy{};
	recv_var_proxy_fn m_proxy_function{};
	data_table_recv_var_proxy_fn m_data_table_proxy_function{};
	recv_table_t* m_data_table{};
	int m_offset{};
	int m_element_stride{};
	int m_elements{};
	const char* m_parent_array_prop_name{};

	[[nodiscard]] recv_var_proxy_fn get_proxy_fn( ) const;
	void set_proxy_fn( recv_var_proxy_fn fn );
	[[nodiscard]] data_table_recv_var_proxy_fn get_data_table_proxy_fn( ) const;
	void set_data_table_proxy_fn( data_table_recv_var_proxy_fn fn );
};

struct recv_table_t {
	recv_prop_t* m_props_pointer{};
	int m_props{};
	void* m_decoder{};
	char* m_net_table_name{};
	bool m_initialized{};
	bool m_in_main_list{};
};

inline recv_var_proxy_fn recv_prop_t::get_proxy_fn( ) const {
	return m_proxy_function;
}

inline void recv_prop_t::set_proxy_fn( const recv_var_proxy_fn fn ) {
	m_proxy_function = fn;
}

inline data_table_recv_var_proxy_fn recv_prop_t::get_data_table_proxy_fn( ) const {
	return m_data_table_proxy_function;
}

inline void recv_prop_t::set_data_table_proxy_fn( const data_table_recv_var_proxy_fn fn ) {
	m_data_table_proxy_function = fn;
}
