//#include "config_manager.h"
//#include "../obfuscation/xor.h"
//#include "../utils.h"
//#include <ostream>
//#include <fstream>
//
//using namespace nlohmann;
//
//void c_config_manager::test()
//{
//	push_item(&g_cfg.m_text.test_var, false, _("Text"));
//}
//
//void c_config_manager::push_item(float* pointer, const float value, const std::string& name)
//{
//	m_items.push_back(new c_item(std::string(name), pointer, _("float")));
//	*pointer = value;
//}
//
//void c_config_manager::push_item(key_bind_t* pointer, const key_bind_t value, const std::string& name)
//{
//	m_items.push_back(new c_item(std::string(name), pointer, _("key_bind")));
//	*pointer = value;
//}
//
//void c_config_manager::push_item(int* pointer, const int value, const std::string& name)
//{
//	m_items.push_back(new c_item(std::string(name), pointer, _("int")));
//	*pointer = value;
//}
//
//void c_config_manager::push_item(bool* pointer, const bool value, const std::string& name)
//{
//	m_items.push_back(new c_item(std::string(name), pointer, _("bool")));
//	*pointer = value;
//}
//
//void c_config_manager::save_config(const std::string& config) const
//{
//	json all_json;
//
//	for (const auto it : m_items)
//	{
//		json j;
//
//		j[_("name")] = it->name;
//		j[_("type")] = it->type;
//
//		if (!it->type.compare(_("int")))
//			j[_("value")] = *static_cast<int*>(it->pointer);
//		else if (!it->type.compare(_("float")))
//			j[_("value")] = *static_cast<float*>(it->pointer);
//		else if (!it->type.compare(_("bool")))
//			j[_("value")] = *static_cast<bool*>(it->pointer);
//		else if (!it->type.compare(_("key_bind")))
//		{
//			auto k = *static_cast<key_bind_t*>(it->pointer);
//
//			std::vector a = { k.m_key_selected, k.m_mode_selected };
//			json ja;
//
//			for (auto& i : a)
//				ja.push_back(i);
//
//			j[_("value")] = ja.dump();
//		}
//
//		all_json.push_back(j);
//	}
//
//	std::ofstream out_file((static_cast<std::string>(_("C:\\Digital\\Cfg\\")) + static_cast<std::string>(config)) + ".cfg", std::ios::out | std::ios::trunc);
//	out_file << all_json.dump(4);
//	out_file.close();
//}
