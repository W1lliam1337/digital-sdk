#pragma once

using crc32_t = unsigned long;

void crc32_init(crc32_t* pul_crc);
void crc32_process_buffer(crc32_t* pul_crc, const void* p, int len);
void crc32_final(crc32_t* pul_crc);
crc32_t crc32_get_table_entry(unsigned int slot);

inline crc32_t crc32_process_single_buffer(const void* p, const int len)
{
	crc32_t crc;

	crc32_init(&crc);
	crc32_process_buffer(&crc, p, len);
	crc32_final(&crc);

	return crc;
}
