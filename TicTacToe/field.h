//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#ifndef H_FIELD_T3_INCLUDED_
#define H_FIELD_T3_INCLUDED_

#include "utils.h"

#define FIELD_SIZE   9
#define FIELD_WIDTH  3
#define FIELD_HEIGHT 3

enum t3_tag_Cell
{
    EMPTY,
    PLAYER_1,
    PLAYER_2
};

enum t3_tag_FieldStatus
{
    IN_PROGRESS,
    END_WIN_P1,
    END_WIN_P2,
    END_DRAW
};

struct t3_tag_Field
{
    Cell cell[FIELD_SIZE];
    int  empty_cell_count;
    char symbol_player_1;
    char symbol_player_2;
};

/*  operation:      display game's field on screen  */
/*  preconditions:  field points to game's field    */
/*  postconditions: field displayed in console      */
void            t3FieldInit                 (Field * field, char symbol_player_1, char symbol_player_2);
void            t3FieldCopy                 (Field * dest, Field const * src);

/*  operation:      display game's field on screen  */
/*  preconditions:  field points to game's field    */
/*  postconditions: field displayed in console      */
void            t3FieldShow                 (Field const * field);

FieldStatus     t3FieldGetStatus            (Field const * field);

#endif