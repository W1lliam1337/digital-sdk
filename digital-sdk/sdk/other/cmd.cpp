#include "cmd.h"
#include "../../utils/utils.h"
#include "../../utils/modules/modules.h"

std::uint32_t c_user_cmd::get_checksum() const
{
	static auto sig = reinterpret_cast<std::uint32_t(__thiscall*)(decltype(this))>(utils::sig(
		modules::m_client_dll, _("53 8B D9 83 C8")));
	return sig(this);
}