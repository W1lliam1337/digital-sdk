#pragma once
#include "../sdk.hpp"

using create_interface_fn = void* (*)(const char*, int*);

class c_interfaces : public c_singleton<c_interfaces>
{
public:
	void init() const;
	void* get_interface(HMODULE module, const char* interface_name) const;
};

