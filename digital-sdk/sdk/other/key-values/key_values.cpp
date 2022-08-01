#include "key_values.h"
#include "../../../utils/utils.h"
#include "../../../utils/modules/modules.h"

c_key_values::c_key_values(const char* key_name, void* unk, void* unk2)
{
    static const auto init_address = utils::sig(modules::m_client_dll, _("55 8B EC 56 8B F1 33 C0 8B 4D 0C 81"));
    reinterpret_cast<void(__thiscall*)(void*, const char*, void*, void*)>(init_address)(this, key_name, unk, unk2);
}

void c_key_values::load_from_buffer(const char* resource_name, const char* buf, void* file_sys, const char* path_id, void* eval_sym_proc, void* unk)
{
    static const auto key_values_load_from_buffer = utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"));
    return reinterpret_cast<void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*)>(key_values_load_from_buffer)(this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
}