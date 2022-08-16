//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#include "utils.h"

#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)

#include <Windows.h>

#define DEF_WINDOWS_T3_

static CONSOLE_SCREEN_BUFFER_INFO           csbi;
static HANDLE                               hStdOut;
static HANDLE                               hStdIn;
static DWORD                                count;
static DWORD                                bufferSize;
static const COORD                          startConsoleCoords = {0, 0};

static bool     t3WinPrepareIO              (void);

static Key      t3WinInputGet               (char const * prompt);

static void     t3WinScreenClear            (void);

#elif defined(__linux__) || defined(__unix__)

#include <unistd.h>
#include <term.h>
#include <termios.h>

#define DEF_UNIX_T3_

#define MAGIC_MAX_CHARS 18

struct termios  initial_settings;
struct termios  settings;
unsigned char   keycodes[MAGIC_MAX_CHARS];
int             count;

static bool     t3UNIXPrepareIO             (void);

static Key      t3UNIXInputGet              (char const * prompt);

static void     t3UNIXScreenClear           (void);

#else
#error Unidentified OS: No input/output functions 
#endif


//==================================================//
//                                                  //
//                  PUBLIC SECTION                  //
//                                                  //
//==================================================//

bool            t3OSPrepareIO               (void)
{
#ifdef DEF_WINDOWS_T3_
    return t3WinPrepareIO();
#elif DEF_UNIX_T3_
    return t3UNIXPrepareIO();
#else
#error Undefined function: InputClear
#endif
}

Key             t3InputGet                  (char const * prompt)
{
#ifdef DEF_WINDOWS_T3_
    return t3WinInputGet(prompt);
#elif DEF_UNIX_T3_
    return t3UNIXInputGet(prompt);
#else
#error Undefined function: InputGet
#endif
}

void            t3ScreenClear               (void)
{
#ifdef DEF_WINDOWS_T3_
    t3WinScreenClear();
#elif DEF_UNIX_T3_
    t3UNIXScreenClear();
#else
#error Undefined function: ScreenClear
#endif
}

void            t3Waiting                   (float seconds)
{
    clock_t delay = (clock_t)(seconds * CLOCKS_PER_SEC);  // convert to clock ticks
    clock_t start = clock();
    while (clock() - start < delay)  // wait until time elapses
        ;                            // note the semicolon
}

int             t3RandomInt                 (int lower, int upper)
{
    srand((unsigned int)time(T3_NULLPTR));
    return lower + rand() % (upper - lower + 1);
}

//==================================================//
//                                                  //
//                 PRIVATE SECTION                  //
//                                                  //
//==================================================//

#ifdef DEF_WINDOWS_T3_

bool            t3WinPrepareIO              (void)
{
    if ((hStdOut = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;
    if ((hStdIn = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;
    return true;
}

Key             t3WinInputGet               (char const * prompt)
{
    DWORD  mode;
    Key    result = T3_KEY_UNDEFINED;

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
        case 0x31:          result = T3_KEY_NUM_1;       break;
        case VK_NUMPAD2:
        case 0x32:          result = T3_KEY_NUM_2;       break;
        case VK_NUMPAD3:
        case 0x33:          result = T3_KEY_NUM_3;       break;
        case VK_NUMPAD4:
        case 0x34:          result = T3_KEY_NUM_4;       break;
        case VK_NUMPAD5:
        case 0x35:          result = T3_KEY_NUM_5;       break;
        case VK_NUMPAD6:
        case 0x36:          result = T3_KEY_NUM_6;       break;
        case VK_NUMPAD7:
        case 0x37:          result = T3_KEY_NUM_7;       break;
        case VK_NUMPAD8:
        case 0x38:          result = T3_KEY_NUM_8;       break;
        case VK_NUMPAD9:
        case 0x39:          result = T3_KEY_NUM_9;       break;
        }
    }
    return result;
}

void            t3WinScreenClear            (void)
{
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;
    bufferSize = csbi.dwSize.X *csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(
            hStdOut, (TCHAR) ' ',
            bufferSize, startConsoleCoords, &count)) 
        return;

    if (!FillConsoleOutputAttribute(
            hStdOut, csbi.wAttributes,
            bufferSize, startConsoleCoords, &count))
        return;

    SetConsoleCursorPosition(hStdOut, startConsoleCoords);
}

#elif DEF_UNIX_T3_

bool            t3UNIXPrepareIO             (void)
{
    return TRUE;
}

Key             t3UNIXInputGet              (char const * prompt)
{
    Key    result = T3_KEY_UNDEFINED;

    tcgetattr( STDIN_FILENO, &initial_settings );
    settings = initial_settings;

    /* Set the console mode to no-echo, raw input. */
    /* The exact meaning of all this jazz will be discussed later. */
    settings.c_cc[ VTIME ]  = 1;
    settings.c_cc[ VMIN  ]  = MAGIC_MAX_CHARS;
    settings.c_iflag       &= ~(IXOFF);
    settings.c_lflag       &= ~(ECHO | ICANON);
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    if(prompt)
        printf("%s", prompt);

    count = read(stdin, (void*)keycodes, MAGIC_MAX_CHARS);

    tcsetattr(STDIN_FILENO, TCSANOW, &initial_settings);

    int k = (count == 1) ? keycodes[0] : -(int)(keycodes[count - 1]);
    switch (k)
    {
    case KEY_1: result = T3_KEY_NUM_1;  break;
    case KEY_2: result = T3_KEY_NUM_2;  break;
    case KEY_3: result = T3_KEY_NUM_3;  break;
    case KEY_4: result = T3_KEY_NUM_4;  break;
    case KEY_5: result = T3_KEY_NUM_5;  break;
    case KEY_6: result = T3_KEY_NUM_6;  break;
    case KEY_7: result = T3_KEY_NUM_7;  break;
    case KEY_8: result = T3_KEY_NUM_8;  break;
    case KEY_9: result = T3_KEY_NUM_9;  break;
    }
    return result;
}

void            t3UNIXScreenClear           (void)
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