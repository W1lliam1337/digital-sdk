# Internal CS:GO cheat base ![CS:GO](https://img.shields.io/badge/game-CS%3AGO-orange?style=flat) ![LIC](https://img.shields.io/github/license/W1lliam1337/digital-sdk) ![LANG](https://img.shields.io/badge/language-C%2B%2B-brightgreen?style=flat )
![App menu](https://i.imgur.com/uWYWscE.png)

# Features
- player esp (box, name)
- bunnyhop
- engine-prediction
- autowall
- chams

# Hooks
- createmove
- reset
- lock cursor
- dme
- cl move
- etc

# Comments navigation
- @note:
- @ida:
- @xref:
- @credits:
- @fix-me:

# Dependencies
 - [imgui](https://github.com/ocornut/imgui)
 - [minhook](https://github.com/TsudaKageyu/minhook)

# Code style example
```c++
class c_class final {
    struct local_struct {
        bool m_var{ };    
    };
public:
    void test( [[maybe_unused]] const int a1 ) const;
}

void c_class::test( const int a1 ) const {
    const auto local = c_player::get_local( );
    if ( !local || !local->is_alive( ) )
        return;

    static const auto sig = g_modules->m_client_dll.get_address( _( "SigName" ) ).offset( 0x1 ).as< fn_t >( );
}
```

# Compatible with both Clang and MSVC compilers #
