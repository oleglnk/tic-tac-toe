//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#ifndef H_FIELD_INCLUDED_
#define H_FIELD_INCLUDED_

#define FIELD_SIZE   9
#define FIELD_WIDTH  3
#define FIELD_HEIGHT 3

//#define TIPS_ENABLED

typedef enum Cell_tag
{
    cell_empty,
    cell_player1,
    cell_player2
} Cell;

typedef enum FieldStatus_tag
{
    fs_in_progress,
    fs_end_win_p1,
    fs_end_win_p2,
    fs_end_draw
} FieldStatus;

typedef struct Field_tag
{
    Cell cell[FIELD_SIZE];
    int  empty_cell_count;
    char symbol_player_1;
    char symbol_player_2;
} Field;


void            t3FieldInit         (Field * field, char symbol_player_1, char symbol_player_2);
void            t3FieldCopy         (Field * dest, Field const * src);
void            t3FieldShow         (Field const * field);
FieldStatus     t3FieldGetStatus    (Field const * field);

#endif