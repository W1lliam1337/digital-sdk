#ifndef VALVE_HANDLE_ENT
#define VALVE_HANDLE_ENT

class c_base_handle;

class c_handle_entity {
public:
    virtual ~c_handle_entity( ) {}

    virtual void set_ref_ehandle( const c_base_handle& handle ) = 0;
    virtual const c_base_handle& get_ref_ehandle( ) const = 0;
};

#endif