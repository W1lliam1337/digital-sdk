# digital-sdk

# language 
- c++ 20
#
# code style 
- snake case
#

# comments navigation
1. `@note:`
2. `@ida:`
3. `@ref`
4. `@credits:`
5. `@fix-me:`
6. `@uc:`
#

# conventions
1. prefix classes with 'c_', interfaces classes with 'i', enums with 'e_' and postfix structures with '_t'
```cpp
class c_class;
class i_interface;
struct struct_t;
enum e_enum;
```
#


## dependencies
 - [imgui](https://github.com/ocornut/imgui)
 - [minhook](https://github.com/TsudaKageyu/minhook)
 - [json](https://github.com/nlohmann/json/)
 - etc
#

## features
- anti-aimbot (yaw and pitch modifiers)
- player esp (box, name, health bar and text)
- bunnyhop
- log system
#

# hooks
- createmove
- reset
- present
- lock cursor
- paint traverse
- frame stage notify
#

![App menu](https://i.imgur.com/nHQW7MP.png)
