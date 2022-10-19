//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#ifndef H_IO_INCLUDED_
#define H_IO_INCLUDED_

#include <stdio.h>
#include <stdlib.h>

//TODO: Delete in C23
#include <stdbool.h>
#define nullptr ((void*)0)

typedef enum Key_tag
{
    t3_key_None,
    t3_key_num1,
    t3_key_num2,
    t3_key_num3,
    t3_key_num4,
    t3_key_num5,
    t3_key_num6,
    t3_key_num7,
    t3_key_num8,
    t3_key_num9
} Key;

bool    t3IOInit(void);

Key		t3GetKey(char const * prompt);

void    t3ScreenClear(void);

#endif