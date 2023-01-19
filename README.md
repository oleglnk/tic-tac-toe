## Introduction

This is one more impementation of popular game TicTacToe written on C. This project was created for 
educational proposes (first of all for me and maybe someone else who will be interested in this) and 
experiments. The most interesting part of the project for programmers is minimax algorithm 
[(wiki link)](https://en.wikipedia.org/wiki/Minimax). Also the project supports Windows and Linux.

## Structure

There are 5 modules, each located respectively in own .h & .c files:
- "Input & Output" module contains functions for screen clear & getting keys from keyboard for 
Windows & Linux.
- Player module contains data & functions about player & AI.
- Game module contains data & functions about game's statue, loop and so on.
- Field module contains data & functions about game's field.
- Game interface (TicTacToe.c) contains functions for different menu screens and function main. The 
project is not big enough to have a separate file for this.

## Compiling

The project was created in Visual Studio 2019 written on C17.
Tested on Windows (MSVC Win32, MSVC x64, MinGW x64) and Linux Mint (gcc).

### Windows

Just run **Visual Studio** and it helps you if something wrong. Also there is a file 
_settings.props_ which is a Property Sheet for Visual Studio. It contatins settings extracted from 
TicTacToe.vcxproj. This simplifies project configuration. I recommend to look at files _.props_, 
_.vcxproj_, _.vcxproj.filters_, _.sln_ for all who starting or working with Visual Studio.

### Linux

To compile on Linux you need GCC.

1. Open terminal.
2. If you haven't gcc - install it and all the necessary libraries for development under Linux.
2. run `cd [your_path_to_source_code_directory]` in terminal.
3. run `gcc -o TicTacToe TicTacToe.c field.c game.c io.c player.c field.h game.h io.h player.h` to 
compile code to file named `TicTacToe` (you can use other names)

Congratulations game is ready to start, just run `./TicTacToe` to run it.

## Known Issues

1. This implementaion of minimax algorithm is not fully optimal and there is a small bug. Also this 
bug easy to find in different impementations which are very popular results in google search. 
I don't want to look for a reason (I already hate this code). If you tell me the reason I will be 
grateful to you for this. I don't tell where is a bug - try to find it by yourself.
2. Normal AI is very dumb, even more dumber than Easy AI. There is an idea how to fix it (thanks to 
my friend) but as I already said: I already hate this code. Your ideas are welcome.
3. **This is not issue of my project. It's a bug of Visual Studio and MSVC compiler.** If you change 
_uint_ to *size_t* in typedef TakeTurn _player.h_ then you will have an errors, a lot of senseless 
errors. GCC is more sane and tell you that you forget to add `#include <stddef.h>`, and that's true.
However, if you try to use x64 version of MSVC you won't get any errors at all. GCC rejects to 
compile and will be right, because this behavior of MSVC is against the C standard.

If you find any other issues, please tell me.

## TODO

Passionately wait for C23 to delete unnecessary code.