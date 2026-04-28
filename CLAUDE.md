# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ShitEngine is a simple 2D game engine where **ShitLang** is the primary scripting language. It is built in C++ on top of **MayEngine** (a separate personal project). ShitLang source files (`.shit`) are compiled to Lua bytecode and run inside MayEngine's integrated Lua 5.4 environment. The project is **Windows-only**.

## Build

**Prerequisites:**
- Visual Studio 2022 with Windows SDK 10.0.19041.0+
- CMake 3.17+, Ninja
- Boost 1.90.0 (cmake version): extract to `ext/boost/`
- Recommended IDE: CLion + CMake + Ninja

**Release build** (from a VS Developer Command Prompt):
```bat
BUILD_RELEASE.BAT
```
This runs CMake with `-DCMAKE_BUILD_TYPE=Release -GNinja` and builds targets `ShitEditor SEPackage MayGame`. Output goes to `build/bin/`.

**Packaging ShitLang code and assets** (after building):
```bat
tools\createPackage.bat
```
This compiles all `.shit` files to bytecode with `ShitCompilerTool.exe`, then packs everything into `data.mp` (encryption key: `ShitPak`) using `MayPack.exe`.

**Distribution requires:**
```
d3dcompiler_47.dll  data.mp  libEGL.dll  libGLESv2.dll  MayEngine.dll  ShitGame.exe
lua/  (optional, needed for network/debugger features)
```

## Architecture

### Build Targets
| Target | Output | Entry Point |
|--------|--------|-------------|
| `MayGame` | `ShitGame.exe` | `MayGame/src/gameMain.cpp` |
| `ShitEditor` | `ShitEditor.exe` | `ShitEditor/src/main.cpp` |
| `SEPackage` / `ShitCompilerTool` | `ShitCompilerTool.exe` | `MayGame/ext/ShitLang/tool/tool.cpp` |
| `ShitCompiler` | `ShitCompiler.exe` | `MayGame/ext/ShitLang/tool/compiler.cpp` |

### Runtime Flow
1. `ShitGame.exe` initializes MayEngine (800×600 window)
2. Loads all game code + assets from `data.mp` (encrypted archive) via `ResourceManager`
3. Sets up Lua environment with MayEngine API bindings (`me.*` namespace) + ShitLang aliases (`shitenv.luac`)
4. Executes `main.shit` — the ShitLang entry point

### Key Directories
- `MayGame/src/` — C++ runtime: engine init, Lua loader (`loadShit.cpp`)
- `MayGame/ext/ShitLang/` — ShitLang compiler toolchain and Lua 5.4 source
- `MayGame/ext/ShitLang/shitenv/shitenv.lua` — defines ShitLang keyword aliases
- `MayGame/Package/shit/shit/` — ShitLang game source files (`.shit`)
- `MayGame/Package/res/` — Assets (shaders, fonts, pipeline configs, Lua scripts)
- `ShitEditor/` — Editor application (IDE for ShitLang)
- `ext/include/mayEngine/` — MayEngine SDK headers
- `ext/bin/Common/` — Pre-built DLLs (MayEngine, OpenGL ES, Direct3D)
- `doc/` — API references (MayEngine CHM, pipeline config format, ShitLang API list)

### Resource System
All assets are accessed at runtime via `ResourceManager` using path strings like `"/shit/main.shit@data.mp"` or `"/font/zpix.ttf@data.mp"`. The `@data.mp` suffix identifies the archive.

## ShitLang

ShitLang is Lua 5.4 with renamed keywords. The compiler (`ShitCompiler.exe`) preprocesses `.shit` files by substituting keywords before compilation.

| ShitLang | Lua equivalent | Meaning |
|----------|---------------|---------|
| `van` | `local` | local variable |
| `fuck` | `function` | function definition |
| `bug` | `end` | block end |
| `ufo … fly` | `for … do` | for loop header |
| `ring … fly` | `while … do` | while loop header |
| `nonono` | `nil` | nil |
| `shit()` | `print()` / `mlog()` | output/logging |
| `tobeng()` | `tostring()` | convert to string |
| `dian` | `table` | table library |
| `budong` | `math` | math library |
| `beng` | `string` | string library |

**Class definition pattern:**
```lua
van MyClass = me.class("MyClass", me.Node, fuck() return me.Node.create() bug)

fuck MyClass.create(cfg)
    van obj = MyClass.new()
    obj:init()
    return obj
bug

fuck MyClass:init()
    -- setup
bug
```

**Import pattern:**
```lua
van MyModule = require("path/to/file.shit@data.mp")
```

## MayEngine API (exposed as `me.*` in ShitLang)

Key globals available in ShitLang scripts:

- `me.Engine` — engine control (`runWithScene`, `loadPipeline`, `exit`, `setFrameRate`, …)
- `me.ScreenManager` — window/display control
- `me.InputManager` — keyboard/mouse input (`checkKeyState`, `getCursorPos`, …)
- `me.v3(x,y,z)` — Vec3 constructor
- `me.color8(r,g,b)` — byte-color Vec3 helper
- `me.Node`, `me.Scene`, `me.Camera` — scene graph nodes
- `me.RendRectangleComponent`, `me.RendSpriteComponent`, `me.RendTextComponent`, `me.RendCircleComponent`, `me.RendLineComponent` — rendering components
- `me.ResourceManager` — load assets from `.mp` archives
- `me.MayText` — font/text rendering
- `me.AudioClip`, `me.AudioPlayer` — audio playback
- `me.PhysicsWorldComponent`, `me.RigidBodyNode`, `me.PhysicsShapeBox`, `me.PhysicsShapeCircle` — 2D physics (Box2D)
- `me.class(name, base, constructor)` — class system
- `mlog` — structured logger (`mlog.i(...)`, `mlog.e(...)`, levels: Trace/Debug/Info/Warn/Error)

Full API reference: `doc/ShitLang API映射列表.md`

## Example Game

`MayGame/Package/shit/shit/snake/` contains a complete Snake game demonstrating the engine's patterns: scene setup, grid-based game logic, entity linking (linked-list snake body), input handling, UI text, and window resize callbacks.
