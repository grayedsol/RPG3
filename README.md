# RPG Game
RPG game using SDL.

Texture assets are from [Seliel the Shaper's](https://seliel-the-shaper.itch.io/) Mana Seed collection.

## Building
### Dependencies
- SDL3
- SDL3_image
- SDL3_ttf

### Compilation
In the terminal:

Make a build directory:
```
cd RPG3
mkdir build
cd build
```

Generate the Buildsystem with CMake.
Set CMAKE_PREFIX_PATH to the directory containing the SDL3 dependencies, if needed:
```
cmake -S ../ -B . -DCMAKE_PREFIX_PATH=/home/grayedsol/.local/
```

Build:
```
cmake --build .
```

This will create the executable, named `game`.