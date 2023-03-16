#include "utils.hh"
#include "../../deps/imgui/imgui.h"

std::wstring c_utils::get_fonts_folder_path( ) {
	PWSTR path{ };
	if ( const HRESULT hr = SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &path ); FAILED( hr ) )
		return { };

	std::wstring fonts_path = path;
	CoTaskMemFree( path );
	return fonts_path;
}

ImFont* c_utils::add_font_from_file( const std::wstring& filename, const float font_size, const ImFontConfig cfg ) {
	const auto& size = WideCharToMultiByte( CP_UTF8, 0, filename.c_str( ), -1, nullptr, 0, nullptr, nullptr );
	std::string str( size, 0 );
	WideCharToMultiByte( CP_UTF8, 0, filename.c_str( ), -1, str.data( ), size, nullptr, nullptr );

	return ImGui::GetIO( ).Fonts->AddFontFromFileTTF( str.c_str( ), font_size, &cfg );
}