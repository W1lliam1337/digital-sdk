#include "netvar_manager.h"
#include <ostream>
#include "../../sdk.hpp"

std::vector<netvar_table>  m_database;

void c_netvars::init()
{
	m_database.clear();

	for (auto clientclass = g_sdk.m_interfaces.m_client->get_all_classes();
		clientclass != nullptr;
		clientclass = clientclass->m_next) 
	{
		if (clientclass->m_recv_table)
			m_database.emplace_back(load_table(clientclass->m_recv_table));
	}
}

netvar_table c_netvars::load_table(const recv_table_t* recv_table) {
	auto table = netvar_table{};

	table.offset = 0;
	table.name = recv_table->m_net_table_name;

	for (auto i = 0; i < recv_table->m_props; ++i) {
		auto prop = &recv_table->m_props_pointer[i];

		if (!prop || isdigit(prop->m_var_name[0]))
			continue;
		if (strcmp(_("baseclass"), prop->m_var_name) == 0)
			continue;

		if (prop->m_recv_type == dpt_datatable && prop->m_data_table) {
			table.child_tables.emplace_back(load_table(prop->m_data_table));
			table.child_tables.back().offset = prop->m_offset;
			table.child_tables.back().prop = prop;
		}
		else {
			table.child_props.emplace_back(prop);
		}
	}
	return table;
}

uint32_t c_netvars::get_offset(const std::string& table_name, const std::string& prop_name) {
	auto result = 0u;
	for (const auto& table : m_database) {
		if (table.name == table_name) {
			result = get_offset(table, prop_name);
			if (result != 0)
				return result;
		}
	}
	return 0;
}

uint32_t c_netvars::get_offset(const netvar_table& table, const std::string& prop_name) {
	for (const auto& prop : table.child_props) {
		if (strncmp(prop->m_var_name, prop_name.data(), prop_name.size()) == 0) {
			return table.offset + prop->m_offset;
		}
	}
	for (const auto& child : table.child_tables) {
		const auto prop_offset = get_offset(child, prop_name);
		if (prop_offset != 0)
			return table.offset + prop_offset;
	}
	for (const auto& child : table.child_tables) {
		if (strncmp(child.prop->m_var_name, prop_name.data(), prop_name.size()) == 0) {
			return table.offset + child.offset;
		}
	}
	return 0;
}

recv_prop_t* c_netvars::get_netvar_prop(const std::string& table_name, const std::string& prop_name) {
	recv_prop_t* result = nullptr;
	for (const auto& table : m_database) {
		if (table.name == table_name) {
			result = get_netvar_prop(table, prop_name);
		}
	}
	return result;
}

recv_prop_t* c_netvars::get_netvar_prop(const netvar_table& table, const std::string& prop_name) {
	for (const auto& prop : table.child_props) {
		if (strncmp(prop->m_var_name, prop_name.data(), prop_name.size()) == 0) {
			return prop;
		}
	}
	for (const auto& child : table.child_tables) {
		const auto prop = get_netvar_prop(child, prop_name);
		if (prop != nullptr)
			return prop;
	}
	for (const auto& child : table.child_tables) {
		if (strncmp(child.prop->m_var_name, prop_name.data(), prop_name.size()) == 0) {
			return child.prop;
		}
	}
	return nullptr;
}
