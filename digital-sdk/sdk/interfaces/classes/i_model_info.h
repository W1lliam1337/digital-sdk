#pragma once
#include "../../../utils/utils.h"

class studiohdr_t;
struct model_t;

class i_model_info
{
public:
	model_t* get_model(const int index)
	{
		return utils::call_vfunc<model_t* (__thiscall*)(i_model_info*, int)>(this, 1)(this, index);
	}

	int get_model_index(const char* filename)
	{
		return utils::call_vfunc<int(__thiscall*)(i_model_info*, const char*)>(this, 2)(this, filename);
	}

	const char* get_model_name(const void* model)
	{
		return utils::call_vfunc<const char* (__thiscall*)(i_model_info*, const void*)>(this, 3)(this, model);
	}

	studiohdr_t* get_studio_model(const model_t* model)
	{
		return utils::call_vfunc< studiohdr_t* (__thiscall*)(i_model_info*, const model_t*)>(this, 32)(this, model);
	}
};
