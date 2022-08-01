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
#

# Hooks
- createmove
- reset
- present
- lock cursor
- paint traverse
- modify_eye_position
- inferno_client_think
- etc
#

# Comments navigation
- `@note:`
- `@ida:`
- `@xref:`
- `@credits:`
- `@fix-me:`
#

# Conventions
Prefix classes with 'c_', interfaces classes with 'i', enums with 'e_' and postfix structures with '_t'
```cpp
class c_class;
class i_interface;
struct struct_t;
enum e_enum;
```
#

# Dependencies
 - [imgui](https://github.com/ocornut/imgui)
 - [minhook](https://github.com/TsudaKageyu/minhook)
 - [json](https://github.com/nlohmann/json/)
 - etc
#

# Code style example
```cpp
struct some_struct_t
{
    bool m_var{};
};

class c_some_class
{
public:
    bool m_var{};
};

enum e_some_enum
{
    create_move_index = 22,
    paint_traverse_index = 41
};

// @ note: test function
void c_self_class::test_func(const int index) noexcept
{
    const auto local = ctx::local();
    const auto eye_pos = ctx::local_data::m_eye_pos;
    static const auto setup_bones_sig = static_cast<void*>(utils::sig(modules::m_client_dll, _("55 8B EC 83 E4 F0 B8 D8")));
}
```
