#include "render.hh"

void c_render::init( ) {
    ImGui::CreateContext( );

    if ( !ImGui_ImplDX9_Init( g_interfaces->m_device ) || !ImGui_ImplWin32_Init( g_ctx->hwnd( ) ) )
        return;

    auto& io = ImGui::GetIO( );
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui::StyleColorsDark( );

    auto& style = ImGui::GetStyle( );
    style.ScrollbarSize = 13.0f;
    style.WindowTitleAlign = ImVec2{ 0.5f, 0.5f };
    style.WindowRounding = 6.f;

    ImFontConfig cfg;
    cfg.PixelSnapH = false;
    cfg.OversampleH = 5;
    cfg.OversampleV = 5;
    cfg.RasterizerMultiply = 1.2f;

    if ( const auto fonts_folder_path = g_utils->get_fonts_folder_path( ); !fonts_folder_path.empty( ) ) {
        const auto font_path = fonts_folder_path + L"\\verdana.ttf";
        m_fonts.m_esp = g_utils->add_font_from_file( font_path, 13.0f, cfg );
    }

    ImGui::GetIO( ).Fonts->AddFontDefault( );
}

void c_render::line( const float x1, const float y1, const float x2, const float y2, c_color clr,
                     const float thickness ) const {
    m_draw_list->AddLine( ImVec2( x1, y1 ), ImVec2( x2, y2 ), clr.u32( ), thickness );
}

void c_render::rect( const float x, const float y, const float w, const float h, c_color clr,
                     const float rounding ) const {
    m_draw_list->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), clr.u32( ), rounding );
}

void c_render::filled_rect( const float x, const float y, const float w, const float h, c_color color,
                            const float rounding, const int flags ) const {
    m_draw_list->AddRectFilled( ImVec2( x, y ), ImVec2( x + w, y + h ), color.u32( ), rounding, flags );
}

void c_render::filled_rect_gradient( const float x, const float y, const float w, const float h, c_color col_upr_left,
                                     c_color col_upr_right, c_color col_bot_right, c_color col_bot_left ) const {
    m_draw_list->AddRectFilledMultiColor( ImVec2( x, y ), ImVec2( x + w, y + h ), col_upr_left.u32( ),
                                          col_upr_right.u32( ), col_bot_right.u32( ), col_bot_left.u32( ) );
}

void c_render::triangle( const float x1, const float y1, const float x2, const float y2, const float x3, const float y3,
                         c_color clr, const float thickness ) const {
    m_draw_list->AddTriangle( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImVec2( x3, y3 ), clr.u32( ), thickness );
}

void c_render::triangle_filled( const float x1, const float y1, const float x2, const float y2, const float x3,
                                const float y3, c_color clr ) const {
    m_draw_list->AddTriangleFilled( ImVec2( x1, y1 ), ImVec2( x2, y2 ), ImVec2( x3, y3 ), clr.u32( ) );
}

void c_render::circle( const float x1, const float y1, const float radius, c_color col, const int segments ) const {
    m_draw_list->AddCircle( ImVec2( x1, y1 ), radius, col.u32( ), segments );
}

void c_render::circle_filled( const float x1, const float y1, const float radius, c_color col,
                              const int segments ) const {
    m_draw_list->AddCircleFilled( ImVec2( x1, y1 ), radius, col.u32( ), segments );
}

void c_render::text( const vec2_t& txt_pos, c_color color, const int flags, const ImFont* font,
                     const std::string_view& message ) const {
    m_draw_list->PushTextureID( font->ContainerAtlas->TexID );

    const auto size = font->CalcTextSizeA( font->FontSize, FLT_MAX, 0.0f, message.data( ) );
    const auto pos = ImVec2( txt_pos.x - size.x / 2.0f, txt_pos.y );
    auto outline_clr = c_color( 0, 0, 0, static_cast< int >( color.a( ) * 0.3f ) );

    if ( flags & drop_shadow )
        m_draw_list->AddText( ImVec2( pos.x + 1, pos.y + 1 ), outline_clr.u32( ), message.data( ) );

    if ( flags & outline ) {
        constexpr std::array< ImVec2, 8 > offsets = {
            { { 1, -1 }, { -1, 1 }, { -1, -1 }, { 1, 1 }, { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } }
        };

        for ( const auto& offset : offsets )
            m_draw_list->AddText( font, font->FontSize, pos + offset, outline_clr.u32( ), message.data( ) );
    }

    m_draw_list->AddText( font, font->FontSize, pos, color.u32( ), message.data( ) );
}

bool c_render::world_to_screen( const vec3_t& in, vec2_t& out ) {
    const auto& matrix = g_interfaces->m_engine->get_view_matrix( );
    out.x = matrix[ 0 ][ 0 ] * in.x + matrix[ 0 ][ 1 ] * in.y + matrix[ 0 ][ 2 ] * in.z + matrix[ 0 ][ 3 ];
    out.y = matrix[ 1 ][ 0 ] * in.x + matrix[ 1 ][ 1 ] * in.y + matrix[ 1 ][ 2 ] * in.z + matrix[ 1 ][ 3 ];

    const auto w = matrix[ 3 ][ 0 ] * in.x + matrix[ 3 ][ 1 ] * in.y + matrix[ 3 ][ 2 ] * in.z + matrix[ 3 ][ 3 ];
    if ( w < 0.001f ) {
        out *= 100000.f;
        return false;
    }

    out /= w;

    const auto& io = ImGui::GetIO( );
    const auto screen_width = io.DisplaySize.x, screen_height = io.DisplaySize.y;

    out.x = screen_width / 2.f + out.x * screen_width / 2.f;
    out.y = screen_height / 2.f - out.y * screen_height / 2.f;

    return true;
}