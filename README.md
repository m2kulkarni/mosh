
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
    - [ ] Text Background color
    - [ ] Fix scaling between lines
    - [ ] Prefetch Glyphs: regular, bold, italic
- ANSI X3.64
    - [x] Color Escape codes
    - [x] Background escape codes
    - [ ] Font-Style Codes
- Run Commands
    - [x] Run commands
    - [ ] process output
    - [ ] sanitize input
    - [ ] async commands

## End goal?
What should really be the end-goal of this project? \
I like using terminals and so do a lot of people. I sometimes envision a one big superapp powering your own workflow. I want an AI in my OS helping me everywhere i do anything. Customizable.

### User 1: Researcher
I am usually connected to a cluster where i am running experiments. Along with that I am reading a paper while taking notes somewhere (or writing a paper on overleaf), and a chatgpt instance where i ask questions and doubts. \
I am okay with a AI assistant that can read my entire screen and computer state. It knows the context of my paper. It knows what experiment i am running, and it knows im taking notes in latex. \

