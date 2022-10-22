//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#include "io.h"

//=============    PRIVATE SECTION    ==============//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)

#include <Windows.h>

#define T3_WINDOWS_

static CONSOLE_SCREEN_BUFFER_INFO   csbi;
static HANDLE                       hStdOut;
static HANDLE                       hStdIn;
static DWORD                        count;
static DWORD                        bufferSize;
static COORD const                  startConsoleCoords = {0, 0};

static bool    t3IOInitWin(void)
{
    if ((hStdOut = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;
    if ((hStdIn = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;
    return true;
}

static Key     t3GetKeyWin(char const * prompt)
{
    DWORD  mode;
    Key    result = t3_key_None;

    if (GetConsoleMode(hStdIn, &mode) && SetConsoleMode(hStdIn, 0))
    {
        if (prompt)
            // UNICODE option is used, so WinApi functions expect wchar_t params
            // There are 3 options (in fact 5, but project is not so big) :
            // - use Ansi functions explicitly: 
            // WriteConsoleA(hStdOut, prompt, lstrlenA(prompt), &count, NULL);
            // - change chars to wchars 
            // - use C standard library function
            printf("%s", prompt);

        FlushConsoleInputBuffer(hStdIn);

        INPUT_RECORD inrec;
        do
            ReadConsoleInput(hStdIn, &inrec, 1, &count);
        while ((inrec.EventType != KEY_EVENT) ||
            (inrec.Event.KeyEvent.bKeyDown));

        SetConsoleMode(hStdIn, mode);
        switch (inrec.Event.KeyEvent.wVirtualKeyCode)
        {
        case VK_NUMPAD1:
        case 0x31:       result = t3_key_num1; break;
        case VK_NUMPAD2:
        case 0x32:       result = t3_key_num2; break;
        case VK_NUMPAD3:
        case 0x33:       result = t3_key_num3; break;
        case VK_NUMPAD4:
        case 0x34:       result = t3_key_num4; break;
        case VK_NUMPAD5:
        case 0x35:       result = t3_key_num5; break;
        case VK_NUMPAD6:
        case 0x36:       result = t3_key_num6; break;
        case VK_NUMPAD7:
        case 0x37:       result = t3_key_num7; break;
        case VK_NUMPAD8:
        case 0x38:       result = t3_key_num8; break;
        case VK_NUMPAD9:
        case 0x39:       result = t3_key_num9; break;
        }
}
    return result;
}

static void    t3ScreenClearWin(void)
{
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;
    bufferSize = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(
        hStdOut, (TCHAR)' ',
        bufferSize, startConsoleCoords, &count))
        return;

    if (!FillConsoleOutputAttribute(
        hStdOut, csbi.wAttributes,
        bufferSize, startConsoleCoords, &count))
        return;

    SetConsoleCursorPosition(hStdOut, startConsoleCoords);
}

#elif defined(__linux__) || defined(__unix__)

#include <unistd.h>
#include <string.h>
#include <termios.h>

#define T3_UNIX_

struct termios  settings_original;
struct termios  settings_keycatch;

static bool    t3IOInitUnix(void)
{
    tcgetattr(STDIN_FILENO, &settings_original);
    tcgetattr(STDIN_FILENO, &settings_keycatch);

    /* Set the console mode to no-echo, raw input. */
    settings_keycatch.c_iflag &= ~(IXOFF);
    settings_keycatch.c_lflag &= ~(ECHO | ICANON);
    
    return true;
}

static Key     t3GetKeyUnix(char const * prompt)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &settings_keycatch);

    if (prompt)
        printf("%s", prompt);

    int character = fgetc(stdin);

    tcsetattr(STDIN_FILENO, TCSANOW, &settings_original);

    Key result = t3_key_None;
    switch (character)
    {
    case 0x31: result = t3_key_num1;  break;
    case 0x32: result = t3_key_num2;  break;
    case 0x33: result = t3_key_num3;  break;
    case 0x34: result = t3_key_num4;  break;
    case 0x35: result = t3_key_num5;  break;
    case 0x36: result = t3_key_num6;  break;
    case 0x37: result = t3_key_num7;  break;
    case 0x38: result = t3_key_num8;  break;
    case 0x39: result = t3_key_num9;  break;
    }
    return result;
}

static void    t3ScreenClearUnix(void)
{
    printf("\033[2J\033[1;1H");
}

#else
#error Unidentified OS: No input/output functions 
#endif


//==============    PUBLIC SECTION    ==============//

bool    t3IOInit(void)
{
#ifdef T3_WINDOWS_
    return t3IOInitWin();
#elif defined(T3_UNIX_)
    return t3IOInitUnix();
#else
#error Undefined function: InputClear
#endif
}

Key     t3GetKey(char const * prompt)
{
#ifdef T3_WINDOWS_
    return t3GetKeyWin(prompt);
#elif defined(T3_UNIX_)
    return t3GetKeyUnix(prompt);
#else
#error Undefined function: InputGet
#endif
}

void    t3ScreenClear(void)
{
#ifdef T3_WINDOWS_
    t3ScreenClearWin();
#elif defined(T3_UNIX_)
    t3ScreenClearUnix();
#else
#error Undefined function: ScreenClear
#endif
}