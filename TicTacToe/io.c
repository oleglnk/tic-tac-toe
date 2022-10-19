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
            WriteConsole(hStdOut, prompt, lstrlen(prompt), &count, NULL);

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
#include <term.h>
#include <termios.h>

#define T3_UNIX_

#define MAGIC_MAX_CHARS 18

struct termios  initial_settings;
struct termios  settings;
unsigned char   keycodes[MAGIC_MAX_CHARS];
int             count;

static bool    t3IOInitUnix(void)
{
    return TRUE;
}

static Key     t3GetKeyUnix(char const * prompt)
{
    Key    result = T3_KEY_UNDEFINED;

    tcgetattr(STDIN_FILENO, &initial_settings);
    settings = initial_settings;

    /* Set the console mode to no-echo, raw input. */
    /* The exact meaning of all this jazz will be discussed later. */
    settings.c_cc[VTIME] = 1;
    settings.c_cc[VMIN] = MAGIC_MAX_CHARS;
    settings.c_iflag &= ~(IXOFF);
    settings.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);

    if (prompt)
        printf("%s", prompt);

    count = read(stdin, (void*)keycodes, MAGIC_MAX_CHARS);

    tcsetattr(STDIN_FILENO, TCSANOW, &initial_settings);

    int k = (count == 1) ? keycodes[0] : -(int)(keycodes[count - 1]);
    switch (k)
    {
    case KEY_1: result = t3_key_num1;  break;
    case KEY_2: result = t3_key_num2;  break;
    case KEY_3: result = t3_key_num3;  break;
    case KEY_4: result = t3_key_num4;  break;
    case KEY_5: result = t3_key_num5;  break;
    case KEY_6: result = t3_key_num6;  break;
    case KEY_7: result = t3_key_num7;  break;
    case KEY_8: result = t3_key_num8;  break;
    case KEY_9: result = t3_key_num9;  break;
    }
    return result;
}

static void    t3ScreenClearUnix(void)
{
    if (!cur_term)
    {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if (result <= 0)
            return;
    }

    putp(tigetstr("clear"));
}

#else
#error Unidentified OS: No input/output functions 
#endif


//==============    PUBLIC SECTION    ==============//

bool    t3IOInit(void)
{
#ifdef T3_WINDOWS_
    return t3IOInitWin();
#elif T3_UNIX_
    return t3IOInitUnix();
#else
#error Undefined function: InputClear
#endif
}

Key     t3GetKey(char const * prompt)
{
#ifdef T3_WINDOWS_
    return t3GetKeyWin(prompt);
#elif T3_UNIX_
    return t3GetKeyUnix(prompt);
#else
#error Undefined function: InputGet
#endif
}

void    t3ScreenClear(void)
{
#ifdef T3_WINDOWS_
    t3ScreenClearWin();
#elif T3_UNIX_
    t3ScreenClearUnix();
#else
#error Undefined function: ScreenClear
#endif
}