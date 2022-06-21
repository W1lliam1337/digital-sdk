#pragma once
#include "../../sdk.hpp"

class i_global_vars_base;

using create_client_class = void* (__cdecl*)(int, int);
using create_event = void* (__cdecl*)();

struct client_class_t {

	create_client_class m_create{};
	create_event m_create_event{};
	char* m_network_name{};
	recv_table_t* m_recv_table{};
	client_class_t* m_next{};
	int m_class_id{};
};

class i_base_client_dll
{
public:
	virtual int              connect(uint32_t appSystemFactory, i_global_vars_base* pGlobals) = 0;
	virtual int              disconnect(void) = 0;
	virtual int              init(uint32_t appSystemFactory, i_global_vars_base* pGlobals) = 0;
	virtual void             post_init() = 0;
	virtual void             shutdown(void) = 0;
	virtual void             level_init_pre_entity(char const* pMapName) = 0;
	virtual void             level_init_post_entity() = 0;
	virtual void             level_shutdown(void) = 0;
	virtual client_class_t* get_all_classes(void) = 0;
};