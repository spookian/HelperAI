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

**Do not forget to install Python 3 beforehand. This is the most important part of the process.**

Finally, add the main_new.dol file to a RtDL copy of your choice, and rename it back to main.dol.

At the moment, the mod has not been finalized yet, so please use this supplied Gecko code to hook into the game.

```
044f7fe8 483d9230
048d1298 4BC26D54
044ee6bc 483E2BE0
048d1470 4BC1D258
```

## Usage

Use another controller to spawn a helper. The helper will automatically start moving on its own. 

## Warning
This is VERY UNSTABLE. Crashes WILL happen at any given moment. Pausing will crash the game the moment a helper is spawned.
