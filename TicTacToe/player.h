//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#ifndef H_PLAYER_INCLUDED_
#define H_PLAYER_INCLUDED_

#include "field.h"

#define PLAYERNAME_SIZE_MAX 21

typedef enum PlayerType_tag
{
    pl_human,
    pl_ai_easy,
    pl_ai_norm,
    pl_ai_hard
} PlayerType;

typedef unsigned int uint;
// If you want to change uint to size_t 
//  then add #include <stddef.h>
// MVSC x64 can compile size_t without header <==== BUG
typedef uint (*TakeTurn)(Field const * field);

typedef struct Player_tag
{
    char        name[PLAYERNAME_SIZE_MAX]; //Name
    TakeTurn    turn;                      //Pointer to function of making turn
} Player;


void    t3PlayerFactory(Player * player, PlayerType const type);

#endif