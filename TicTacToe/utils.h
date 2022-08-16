//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#ifndef H_UTILS_T3_INCLUDED_
#define H_UTILS_T3_INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T3_NULLPTR ((void*)0)

typedef enum    t3_tag_Cell                 Cell;
typedef enum    t3_tag_PlayerType           PlayerType;
typedef enum    t3_tag_FieldStatus          FieldStatus;
typedef enum    t3_tag_Key                  Key;

typedef struct  t3_tag_Field                Field;
typedef struct  t3_tag_Player               Player;

enum t3_tag_Key
{
    T3_KEY_UNDEFINED,
    T3_KEY_NUM_1,
    T3_KEY_NUM_2,
    T3_KEY_NUM_3,
    T3_KEY_NUM_4,
    T3_KEY_NUM_5,
    T3_KEY_NUM_6,
    T3_KEY_NUM_7,
    T3_KEY_NUM_8,
    T3_KEY_NUM_9
};

bool    t3OSPrepareIO(void);

Key		t3InputGet(
			char	const * prompt);

void    t3ScreenClear(void);

/*  operation:      delay thread's work             */
/*  preconditions:  seconds of delay                */
void    t3Waiting(
			float	const   seconds);

/*  operation:      get random number in range      */
/*  preconditions:  lower bound of range            */
/*                  upper bound of range            */
/*  postconditions: random number                   */
/*                  in range [lower; upper]         */
int     t3RandomInt(
			int		const   lower, 
			int		const   upper);

#endif