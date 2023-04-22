# HelperAI
Work-in-progress AI partner modification for Kirby's Return to Dreamland.

## Dependencies

[Python 3.6 or above](https://www.python.org/)  
[CMake](https://cmake.org/)  
[Golem](https://github.com/spookian/Golem)  
[devkitPro](https://devkitpro.org/wiki/Getting_Started)

## Compilation

Place a vanilla main.dol and a compiled version of the Golem program into the root of the cloned repository.

```
cmake .
cmake --build .
```

Next, generate a Unix makefile with CMake, and build at the root of the repo.
If all goes well, there should be a main_new.dol generated.

Finally, add the main_new.dol file to a RtDL copy of your choice, and rename it back to main.dol.

## Usage

Press B on your Wii Remote to spawn a computer controlled helper during gameplay.

## Warning
This is VERY UNSTABLE. Crashes WILL happen at any given moment. Pausing will crash the game the moment a helper is spawned.
