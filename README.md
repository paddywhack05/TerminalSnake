# TerminalSnake

## Build instruction
gcc main.c -lncurses -DNCURSES_STATIC\
or\
make\
or use cmake

# Regular Instructions
The Vs code terminal can look iffy with ncurses depending on Os cursor position may only update on input\
If you are using an old version of linux u may need to link rt like -lrt\
If you are not using mingw on windows timing may not work properly its not a native win api
