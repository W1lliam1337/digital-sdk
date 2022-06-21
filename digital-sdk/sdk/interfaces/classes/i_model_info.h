#pragma once
#include "../../../dependencies/vfunc/vfunc.h"

class studiohdr_t;
struct model_t;

class i_model_info {
public:
	model_t* get_model(const int index)
	{
		using o_fn = model_t * (__thiscall*)(i_model_info*, int);
		return call_vfunc<o_fn>(this, 1)(this, index);
	}

	int get_model_index(const char* filename)
	{
		using o_fn = int(__thiscall*)(i_model_info*, const char*);
		return call_vfunc<o_fn>(this, 2)(this, filename);
	}

	const char* get_model_name(const void* model)
	{
		using o_fn = const char* (__thiscall*)(i_model_info*, const void*);
		return call_vfunc<o_fn>(this, 3)(this, model);
	}

	studiohdr_t* get_studio_model(const model_t* model)
	{
		using o_fn = studiohdr_t * (__thiscall*)(i_model_info*, const model_t*);
		return call_vfunc<o_fn>(this, 32)(this, model);
	}
};
