#pragma once

using create_interface_fn = void* (*)(const char* pName, int* pReturnCode);
using instantiate_interface_fn = void* (*)();

class c_app_system
{
public:
	virtual bool connect(create_interface_fn factory) = 0; // 0
	virtual void disconnect() = 0; // 1
	virtual void* query_interface(const char* pInterfaceName) = 0; // 2
	virtual int init() = 0; // 3
	virtual void shutdown() = 0; // 4
	virtual const void* get_dependencies() = 0; // 5
	virtual int get_tier() = 0; // 6
	virtual void reconnect(create_interface_fn factory, const char* pInterfaceName) = 0; // 7
	virtual void unk_func() = 0; // 8
};