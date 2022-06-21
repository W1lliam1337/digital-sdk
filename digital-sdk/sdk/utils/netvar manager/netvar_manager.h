#pragma once
#include <string>
#include <vector>
#include "../../../dependencies/singleton/singleton.h"

struct recv_prop_t;
struct recv_table_t;

struct netvar_table {
	std::string               name{};
	recv_prop_t* prop{};
	uint32_t                  offset{};
	std::vector<recv_prop_t*>    child_props{};
	std::vector<netvar_table> child_tables{};
};

class c_netvars : public c_singleton<c_netvars> {
public:
	void init();

	uint32_t      get_offset(const std::string& table_name, const std::string& prop_name);
	recv_prop_t* get_netvar_prop(const std::string& table_name, const std::string& prop_name);

	netvar_table  load_table(const recv_table_t* recv_table);

	uint32_t      get_offset(const netvar_table& table, const std::string& prop_name);
	recv_prop_t* get_netvar_prop(const netvar_table& table, const std::string& prop_name);
};

#define GET_NETVAR_OFFSET(type, name, table, netvar, offset)                           \
    type& name() const {                                          \
        static int _##name = c_netvars::get()->get_offset(table, netvar) + offset;     \
        return *(type*)((DWORD)this + _##name);                 \
    }

#define GET_NETVAR(type, name, table, netvar)                           \
    type& name() const {                                          \
        static int _##name = c_netvars::get()->get_offset(table, netvar);     \
        return *(type*)((DWORD)this + _##name);                 \
    }


#define GET_OFFSET( name, type, offset ) \
__forceinline type& name( ) \
{\
    return *( type* )( ( uintptr_t )( this ) + offset ); \
}