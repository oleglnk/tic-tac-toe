//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#include "game.h"

#include "field.h"
#include "player.h"

#include "utils.h"

//==================================================//
//                                                  //
//               DECLARATION SECTION                //
//                                                  //
//==================================================//

#define PLAYER_COUNT 2

static Field            field;
static Player *         players[PLAYER_COUNT];
static FieldStatus      game_status;


static void     t3GameInit(Player * player1, Player * player2);
static void     t3GameOver(void);
static void     t3GameLoop(void);


//==================================================//
//                                                  //
//                  PUBLIC SECTION                  //
//                                                  //
//==================================================//

void            t3Game                      (Player * player1, Player * player2)
{
    t3GameInit(player1, player2);
    t3GameLoop();
    t3GameOver();
}

//==================================================//
//                                                  //
//                  PRIVATE SECTION                 //
//                                                  //
//==================================================//

void            t3GameInit                  (Player * player1, Player * player2)
{
    t3FieldInit(&field, 'x', 'o');

    players[0] = player1;
    players[1] = player2;

    game_status = IN_PROGRESS;
}

void t3GameLoop(void)
{
    int player_turn = 0;
    int field_pos;
    
    while (1)
    {
        field_pos = players[player_turn]->turn(&field);
        if (field.cell[field_pos] != EMPTY)
            continue;

        field.cell[field_pos] = (Cell)(player_turn + 1);
        field.empty_cell_count--;       

        if ((game_status = t3FieldGetStatus(&field)) != IN_PROGRESS)
            break;

        player_turn++;
        player_turn %= PLAYER_COUNT;

        t3ScreenClear();
        t3FieldShow(&field);
    }
    t3ScreenClear();
}

void t3GameOver(void)
{
    t3FieldShow(&field);
    puts("\n");

    if (END_DRAW == game_status)
    {
        puts("Wow! That was awesome!");
        puts("Opponents are worthy of each other!");
    }
    else
    {
        puts("Youppi! Yeah!");
        printf("Congradulations to player %s!\n",
               (END_WIN_P1 == game_status) ? players[0]->name : players[1]->name);
        puts("You are winner!!!");
    }
    t3InputGet("Press any key to continue...");
}