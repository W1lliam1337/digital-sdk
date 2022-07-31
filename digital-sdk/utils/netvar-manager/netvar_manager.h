#pragma once
#include <string>
#include <vector>
#include "../../includes.h"

struct recv_prop_t;
struct recv_table_t;

struct net_var_table_t {
	std::string               name{};
	recv_prop_t* prop{};
	uint32_t                  offset{};
	std::vector<recv_prop_t*>    child_props{};
	std::vector<net_var_table_t> child_tables{};
};

namespace net_vars
{
	void init();

	uint32_t      get_offset(const std::string& table_name, const std::string& prop_name);
	recv_prop_t* get_net_var_prop(const std::string& table_name, const std::string& prop_name);

	net_var_table_t  load_table(const recv_table_t* recv_table);

	uint32_t      get_offset(const net_var_table_t& table, const std::string& prop_name);
	recv_prop_t* get_net_var_prop(const net_var_table_t& table, const std::string& prop_name);
}

#define GET_NETVAR_OFFSET(type, name, table, netvar, offset)                           \
    type& name() const {                                          \
        static int _##name = net_vars::get_offset(table, netvar) + offset;     \
        return *(type*)((DWORD)this + _##name);                 \
    }

#define GET_NETVAR(type, name, table, netvar)                           \
    type& name() const {                                          \
        static int _##name = net_vars::get_offset(table, netvar);     \
        return *(type*)((DWORD)this + _##name);                 \
    }


#define GET_OFFSET( type, name, offset ) \
__forceinline type& name( ) \
{\
    return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
}