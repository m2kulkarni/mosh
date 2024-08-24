# GPU-accelerated Terminal Emulator

## Build Instructions

```
mkdir build
cd build
cmake ..
make
./term
```

## What we are doing/what we have done?
- OpenGL tutorials
   - [x] Spawn a window
   - [x] Render triangles
   - [x] Write shaders
- Freetype2
    - [x] Load Fonts, Glyphs, bitmaps
    - [x] Show characters on window
- Text 
    - [x] Render keyboard input
    - [x] Space, Backspace, Enter
    - [x] Cursor shape, cursor position
- ANSI X3.64
    - [x] Color Escape codes
    - [ ] Other escape codes
- Run Commands
    - [x] Run commands
    - [ ] process output
    - [ ] sanitize input
    - [ ] async commands
