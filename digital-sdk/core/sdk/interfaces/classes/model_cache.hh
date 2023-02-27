#ifndef VALVE_MODEL_CACHE
#define VALVE_MODEL_CACHE

#include "../../../utils/utils.hh"

enum e_model_cache_data_type {
    mdlcache_studiohdr = 0,
    mdlcache_studiohwdata,
    mdlcache_vcollide,
    mdlcache_animblock,
    mdlcache_virtualmodel,
    mdlcache_vertexes,
    mdlcache_decodedanimblock
};

enum e_model_cache_flush {
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

class c_model_cache_notify {
public:
    virtual void on_data_loaded( e_model_cache_data_type type, unsigned short handle ) = 0;
    virtual void on_data_unloaded( e_model_cache_data_type type, unsigned short handle ) = 0;
};

class i_model_cache {
public:
    VFUNC( void( __thiscall* )( void* ), lock( ), 33 );
    VFUNC( void( __thiscall* )( void* ), unlock( ), 34 );
};

#endif