//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#ifndef H_PLAYER_T3_INCLUDED_
#define H_PLAYER_T3_INCLUDED_

#include "utils.h"

#define T3_PLAYER_NAME_SIZE_MAX 21

enum t3_tag_PlayerType
{
    HUMAN,
    AI_EASY,
    AI_NORM,
    AI_HARD,
};

typedef int(*TakeTurn)(Field const * field);

struct t3_tag_Player
{
    char        name[T3_PLAYER_NAME_SIZE_MAX]; //Name
    TakeTurn    turn;                          //Pointer to function of making turn
};


void            t3PlayerFactory             (Player       * player, PlayerType type);

#endif