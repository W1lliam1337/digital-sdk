#include "menu.hh"

void c_menu::render( ) {
    if ( !m_opened )
        return;

    ImGui::SetNextWindowSize( ImVec2( 512.0f, 515.0f ), ImGuiCond_Once );
    ImGui::Begin( _( "csgo cheat sdk" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );

    ImGui::BeginChild( _( "tabs" ), ImVec2{ 150, 0 }, true );
    {
        static constexpr const char* tabs[]{ "Rage", "Legit", "Anti-aim", "World esp", "Player esp", "Misc" };

        for ( std::size_t i{ }; i < IM_ARRAYSIZE( tabs ); ++i ) {
            if ( ImGui::Selectable( tabs[ i ], m_selected_tab == i ) ) {
                m_selected_tab = i;
            }
        }
    }
    ImGui::EndChild( );
    ImGui::SameLine( );

    ImGui::End( );
}