# PLEASE INSERT COIN

An interactive C++ / OpenGL / FreeGLUT mini-game for CS0045. A routine 2D snack purchase on a night street turns into a 3D rescue: you operate a maintenance claw inside the vending machine to recover the jammed snack. Three customers make increasingly odd requests; the game ends on a visual clue with no explanation text.

## Files

| File | Purpose |
|---|---|
| `PLEASE_INSERT_COIN.cpp` | Complete source code (single file) |
| `PLEASE_INSERT_COIN.exe` | Compiled Windows executable |
| `libfreeglut.dll` | FreeGLUT runtime library the `.exe` needs |

## Requirements

- Windows 10 or 11 (64-bit) with a graphics driver that supports OpenGL
- To compile only: MSYS2 **UCRT64** GCC and FreeGLUT, as prescribed in Module 01

## Run the compiled `.exe`

1. Put `PLEASE_INSERT_COIN.exe` and `libfreeglut.dll` in the **same folder**.
2. Double-click `PLEASE_INSERT_COIN.exe`.

If Windows reports that `libfreeglut.dll` is missing, the DLL is not beside the `.exe`. A copy is also in `C:\Program Files (x86)\msys64\ucrt64\bin\` on the development machine.

The window opens at 1280 x 720 and can be resized; the picture keeps a 16:9 shape with black bars.

## Compile the `.cpp`

### Option A: VS Code task (course setup)

1. Open the folder containing `PLEASE_INSERT_COIN.cpp` in VS Code with the course `.vscode/tasks.json`.
2. Open `PLEASE_INSERT_COIN.cpp`.
3. Run **Terminal > Run Build Task** (the "Build OpenGL Program" task).
4. Run the `PLEASE_INSERT_COIN.exe` created next to the source file.

### Option B: command line

Open a terminal in the source folder and run:

```bash
g++ PLEASE_INSERT_COIN.cpp -o PLEASE_INSERT_COIN.exe -lfreeglut -lopengl32 -lglu32
```

If `g++` is not found, add the UCRT64 compiler folder to `PATH` first (PowerShell example):

```powershell
$env:PATH = "C:\Program Files (x86)\msys64\ucrt64\bin;" + $env:PATH
g++ PLEASE_INSERT_COIN.cpp -o PLEASE_INSERT_COIN.exe -lfreeglut -lopengl32 -lglu32
```

Adjust the path if MSYS2 is installed elsewhere (for example `C:\msys64\ucrt64\bin`). Run the result with `.\PLEASE_INSERT_COIN.exe`.

### Verified build

The command in Option B was run on this setup and compiled with no warnings under `-Wall -Wextra`:

- Compiler: g++ 16.2.0 (MSYS2 UCRT64, Rev3)
- Libraries: FreeGLUT, OpenGL32, GLU32
- OS: Windows 11

The executable depends on `libfreeglut.dll` (dynamic link) because a static FreeGLUT library is not installed in this setup. The standard Windows C runtime DLLs it also uses are part of Windows.

## Controls

| Input | Action |
|---|---|
| Mouse: click **INSERT COIN** / **HOW TO PLAY** | Start / open purchase help (title screen) |
| Mouse: click the coin slot, then a snack button | Insert a coin, then select the requested snack |
| Mouse: click the delivered snack, then CONTINUE | Collect the item, then move on |
| **W A S D** or **arrow keys** (hold) | Move the claw (left/right and depth) |
| **Space** | Lower the claw to grab; press again to release the carried snack |
| **P** or **Esc** | Pause (rescue only); pause menu has Resume, Reinitialize, Controls, Return to Street |
| **Enter** | Confirm / continue / collect |
| **R** | Retry (only on the RECOVERY UNSUCCESSFUL screen) |
| Up / Down | Move focus in menus |

Bring the target snack over the lit chute (the amber pad) and release it. A missed grab or a release outside the chute costs one of three attempts. There is no timer.

## Known limitations

- The world is drawn directly to the window. The planned low-resolution framebuffer pixelation and optional fog are **not implemented**.
- Text is drawn with fixed-size GLUT bitmap fonts. At very small window sizes some labels are shortened or hidden.
- The final `.exe` was built from the current source; the test cases in the project documentation have not yet been executed and recorded.
- Requires a Windows machine. The key-focus handling uses a Windows-only check.
