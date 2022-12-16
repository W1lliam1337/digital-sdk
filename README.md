# Internal CS:GO cheat base ![CS:GO](https://img.shields.io/badge/game-CS%3AGO-orange?style=flat) ![LIC](https://img.shields.io/github/license/W1lliam1337/digital-sdk) ![LANG](https://img.shields.io/badge/language-C%2B%2B-brightgreen?style=flat )
![App menu](https://i.imgur.com/cxUT5ZY.png)
# Features
- legit-bot (auto fire, silent aim, smooth, fov)
- anti-aimbot (yaw and pitch modifiers)
- player esp (box, name, health bar and text)
- chams
- bunnyhop
- log system
- engine-prediction
- third person
- no scope
- removal of blood and molotov

# Hooks
- createmove
- reset
- present
- lock cursor
- paint traverse
- modify_eye_position
- inferno_client_think
- etc

# Comments navigation
- `@note:`
- `@ida:`
- `@xref:`
- `@credits:`
- `@fix-me:`

# Conventions
Prefix classes with 'c_', interfaces classes with 'i', enums with 'e_' and postfix structures with '_t'
```cpp
class c_class;
class i_interface;
struct struct_t;
enum e_enum;
```

# Dependencies
 - [imgui](https://github.com/ocornut/imgui)
 - [minhook](https://github.com/TsudaKageyu/minhook)
 - [json](https://github.com/nlohmann/json/)
 - etc

# Code style example
```cpp
struct some_struct_t {
    bool m_var{};
};

class c_some_class {
public:
    bool m_var{};
};

enum e_some_enum {
    create_move_index = 22,
    paint_traverse_index = 41
};

// @ note: test function
void c_self_class::test_func( const int index ) noexcept {
    const auto local = ctx::local( );
    const auto eye_pos = ctx::local_data::m_eye_pos;
    static const auto setup_bones_sig = static_cast<void*>(utils::sig( modules::m_client_dll, _( "55 8B EC 83 E4 F0 B8 D8" ) ));
}
```

# Some key notes / FAQ
1. Key notes: This SDK is only x86 compatible as it uses cdecl (naked) operators, which does not support x64 and ARM.

2. Please install DirectX on your target system and include in the visual-studio library-link path + include-link path. I updated this manually for my system by putting the relevant headers + libs into dependencies/DirectX/include and dependencies/DirectX/lib. I do not remember off the top of the head if these libraries are portable across all systems, so be prepared to overwrite this OR update the path and remove.

3. Some of usage of this cheat is undocumented and you'll have to go through the codebase to figure out how different things work. That's OK. Just a heads up that to open the menu, press the "Insert" key according to menu.cpp:~30

4. SDK compiles a DLL which will need to be injected (of course). The choice of injection is up to the user / developer.
