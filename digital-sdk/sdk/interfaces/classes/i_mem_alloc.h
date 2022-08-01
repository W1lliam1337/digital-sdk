#pragma once

typedef size_t(*mem_alloc_fail_handler_t)(size_t);

class i_mem_alloc
{
public:
	virtual ~i_mem_alloc();

	virtual void* allocate(size_t size) = 0;
	virtual void* re_allocate(void* mem, size_t size) = 0;
	virtual void free(void* pMem) = 0;
	virtual void* expand_no_longer_suported(void* mem, size_t size) = 0;
	virtual size_t get_size(void* pMem) = 0;
	virtual void push_alloc_debug_info(const char* file_name, int m_line) = 0;
	virtual void pop_alloc_debug_info() = 0;
	virtual long currect_set_break_alloc(long new_break_alloc) = 0;
	virtual	int currect_set_report(int report_type, int report_mode) = 0;
	virtual int currect_is_valid_heap_report(const void* mem) = 0;
	virtual int currect_is_valid_pointer(const void* mem, unsigned int size, int access) = 0;
	virtual int current_check_memory(void) = 0;
	virtual int current_set_debug_flag(int new_flag) = 0;
	virtual void currect_memory_checkpoint(void* state) = 0;
	virtual void dump_stats() = 0;
	virtual void dump_stats_file_base(char const* file_base) = 0;
	virtual void* currect_set_report_file(int rpt_type, void* file) = 0;
	virtual void* currect_set_report_hook(void* fn_new_hook) = 0;
	virtual int currect_debug_report(int rpt_type, const char* file, int line, const char* sz_module, const char* message) = 0;
	virtual int heap_chk() = 0;
	virtual bool is_debug_heap() = 0;
	virtual void get_actual_debug_info(const char*& file_name, int& nLine) = 0;
	virtual void register_allocation(const char* file_name, int line, int logical_size, int actual_size, unsigned time) = 0;
	virtual void register_de_allocation(const char* file_name, int line, int logical_size, int actual_size, unsigned time) = 0;
	virtual int get_version() = 0;
	virtual void compact_heap() = 0;
	virtual mem_alloc_fail_handler_t set_alloc_fail_handler(mem_alloc_fail_handler_t fn_mem_alloc_fail_handler) = 0;
	virtual void dump_block_stats(void*) = 0;
	virtual void set_stats_extra_info(const char* map_name, const char* m_comment) = 0;
	virtual size_t memory_alloc_failed() = 0;
	virtual int get_debug_info_size() = 0;
	virtual void save_debug_info(void* debug_info) = 0;
	virtual void restore_debug_info(const void* debug_info) = 0;
	virtual void initialize_debug_info(void* debug_info, const char* root_file_name, int line) = 0;
	virtual void global_memory_status(size_t* used_memory, size_t* free_memory) = 0;
};