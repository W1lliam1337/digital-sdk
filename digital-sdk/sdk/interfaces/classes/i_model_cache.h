#pragma once

#include "i_app_sys.h"

class studiohdr_t;
struct studiohwdata_t;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

enum mdl_cache_data_type_t
{
	// Callbacks to Get called when data is loaded or unloaded for these:
	mdlcache_studiohdr = 0,
	mdlcache_studiohwdata,
	mdlcache_vcollide,

	// Callbacks NOT called when data is loaded or unloaded for these:
	mdlcache_animblock,
	mdlcache_virtualmodel,
	mdlcache_vertexes,
	mdlcache_decodedanimblock
};

enum mdl_cache_flush_t
{
	mdlcache_flush_studiohdr = 0x01,
	mdlcache_flush_studiohwdata = 0x02,
	mdlcache_flush_vcollide = 0x04,
	mdlcache_flush_animblock = 0x08,
	mdlcache_flush_virtualmodel = 0x10,
	mdlcache_flush_autoplay = 0x20,
	mdlcache_flush_vertexes = 0x40,

	mdlcache_flush_ignorelock = 0x80000000,
	mdlcache_flush_all = 0xFFFFFFFF
};

class c_mdl_cache_notify
{
public:
	virtual void on_data_loaded(mdl_cache_data_type_t type, unsigned short handle) = 0;
	virtual void on_data_unloaded(mdl_cache_data_type_t type, unsigned short handle) = 0;
};

class i_mdl_cache : public c_app_system
{
public:
	virtual void set_cache_notify(c_mdl_cache_notify* pNotify) = 0;
	virtual unsigned short find_mdl(const char* pMDLRelativePath) = 0;
	virtual int add_ref(unsigned short handle) = 0;
	virtual int release(unsigned short handle) = 0;
	virtual int get_ref(unsigned short handle) = 0;
	virtual studiohdr_t* get_studio_hdr(unsigned short handle) = 0;
	virtual studiohwdata_t* get_hardware_data(unsigned short handle) = 0;
	virtual vcollide_t* get_vcollide(unsigned short handle) = 0;
	virtual unsigned char* get_anim_block(unsigned short handle, int nBlock) = 0;
	virtual virtualmodel_t* get_virtual_model(unsigned short handle) = 0;
	virtual int get_autoplay_list(unsigned short handle, unsigned short** pOut) = 0;
	virtual vertexFileHeader_t* get_vertex_data(unsigned short handle) = 0;
	virtual void touch_all_data(unsigned short handle) = 0;
	virtual void set_user_data(unsigned short handle, void* pData) = 0;
	virtual void* get_user_data(unsigned short handle) = 0;
	virtual bool is_error_model(unsigned short handle) = 0;
	virtual void flush(mdl_cache_flush_t nFlushFlags = mdlcache_flush_all) = 0;
	virtual void flush(unsigned short handle, int nFlushFlags = mdlcache_flush_all) = 0;
	virtual const char* get_model_name(unsigned short handle) = 0;
	virtual virtualmodel_t* get_virtual_model_fast(const studiohdr_t* pStudioHdr, unsigned short handle) = 0;
	virtual void begin_lock() = 0;
	virtual void end_lock() = 0;
	virtual int* get_frame_unlock_counter_ptr_old() = 0;
	virtual void finish_pending_loads() = 0;
	virtual vcollide_t* get_v_collide_ex(unsigned short handle, bool synchronousLoad = true) = 0;
	virtual bool get_vcollide_size(unsigned short handle, int* pVCollideSize) = 0;
	virtual bool get_async_load(mdl_cache_data_type_t type) = 0;
	virtual bool set_async_load(mdl_cache_data_type_t type, bool bAsync) = 0;
	virtual void begin_map_load() = 0;
	virtual void end_map_load() = 0;
	virtual void mark_as_loaded(unsigned short handle) = 0;
	virtual void init_preload_data(bool rebuild) = 0;
	virtual void shutdown_preload_data() = 0;
	virtual bool is_data_loaded(unsigned short handle, mdl_cache_data_type_t type) = 0;
	virtual int* get_frame_unlock_counter_ptr(mdl_cache_data_type_t type) = 0;
	virtual studiohdr_t* lock_studio_hdr(unsigned short handle) = 0;
	virtual void unlock_studio_hdr(unsigned short handle) = 0;
	virtual bool preload_model(unsigned short handle) = 0;
	virtual void reset_error_model_status(unsigned short handle) = 0;
	virtual void mark_frame() = 0;
	virtual void begin_coarse_lock() = 0;
	virtual void end_coarse_lock() = 0;
	virtual void reload_v_collide(unsigned short handle) = 0;
};