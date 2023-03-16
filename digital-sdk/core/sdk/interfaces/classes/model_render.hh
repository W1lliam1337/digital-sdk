#ifndef VALVE_MODEL_RENDER
#define VALVE_MODEL_RENDER

#include "../../misc/collideable.hh"
#include "../../misc/studio.hh"

class i_material;

class c_model_render_info {
public:
    vec3_t m_origin{ };
    vec3_t m_angles{ };
private:
   [[maybe_unused]] std::byte pad[ 0x4 ]{ };
public:
    void* m_renderable{ };
    const model_t* m_model{ };
    const matrix3x4_t* m_model_to_world{ };
    const matrix3x4_t* m_lighting_offset{ };
    const vec3_t* m_lighting_origin{ };
    int m_flags{ };
    int m_entity_index{ };
    int m_skin{ };
    int m_body{ };
    int m_hitbox_set{ };
    unsigned short m_instance{ };

    c_model_render_info( ) {
        m_model_to_world = nullptr;
        m_lighting_offset = nullptr;
        m_lighting_origin = nullptr;
    }
};

struct draw_model_state_t {
    studio_hdr_t* m_studio_hdr{ };
    void* m_studio_hwdara{ };
    c_client_renderable* m_entity{ };
    const matrix3x4_t* m_model_to_world{ };
    void* m_decals{ };
    int m_draw_flags{ };
    int m_lod{ };
};

class i_model_render {
public:
    virtual int draw_model( int flags, void* renderable, unsigned short instance, int entity_index, void* model,
                            vec3_t const& origin, vec3_t const& angles, int skin, int body, int hitboxset,
                            const matrix3x4_t* model_to_world = nullptr,
                            const matrix3x4_t* p_lighting_offset = nullptr ) = 0;
    virtual void force_material( i_material* mat, int n_override_type = 0, int n_overrides = 0 ) = 0;
    virtual bool is_forced_material( void ) = 0;
};

#endif