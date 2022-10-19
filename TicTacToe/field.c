//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#include "field.h"

#include "io.h"

//==============    PUBLIC SECTION    ==============//

void t3FieldInit(
	Field         * field,
	char    const   symbol_player_1, 
	char    const   symbol_player_2)
{
    for (int i = 0; i < FIELD_SIZE; i++)
        field->cell[i] = cell_empty;
    
    field->empty_cell_count = FIELD_SIZE;

    field->symbol_player_1  = symbol_player_1;
    field->symbol_player_2  = symbol_player_2;
}

void t3FieldCopy(
	Field	      * dest,
	Field	const * src)
{
    for (int i = 0; i < FIELD_SIZE; i++)
        dest->cell[i] = src->cell[i];

    dest->empty_cell_count = src->empty_cell_count;

    dest->symbol_player_1 = src->symbol_player_1;
    dest->symbol_player_2 = src->symbol_player_2;
}

void t3FieldShow(
	Field	const * field)
{
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        putchar(' ');
        
        switch (field->cell[i])
        {
        case cell_empty:   putchar(' '); break;
        case cell_player1: putchar(field->symbol_player_1); break;
        case cell_player2: putchar(field->symbol_player_2); break;
        }
        putchar(' ');

        if ((i + 1) % FIELD_WIDTH != 0)
            putchar('|');
        else
        {
            putchar('\n');
            if (i != FIELD_SIZE - 1)
                puts("---+---+---");
        }
    }
#ifdef TIPS_ENABLED
    puts("");
    puts("Enter number of cell from 1 to 9 to make turn.");
    puts("Except already filled cells.");

    for (int i = 0; i < 9; i++)
    {
        putchar(' ');
        printf("%d", (i + 1));
        putchar(' ');

        if ((i + 1) % FIELD_WIDTH != 0)
            putchar('|');
        else
        {
            putchar('\n');
            if (i != FIELD_SIZE - 1)
                puts("---+---+---");
        }
    }
#endif
}

FieldStatus t3FieldGetStatus(
	Field	const * field)
{
    if (field->cell[0])
    {
        if ((field->cell[0] == field->cell[1] && field->cell[0] == field->cell[2]) ||
            (field->cell[0] == field->cell[3] && field->cell[0] == field->cell[6]))
        {
            return (field->cell[0] == cell_player1) ? fs_end_win_p1 : fs_end_win_p2;
        }
    }
    if (field->cell[4])
    {
        if ((field->cell[4] == field->cell[0] && field->cell[4] == field->cell[8]) ||
            (field->cell[4] == field->cell[1] && field->cell[4] == field->cell[7]) ||
            (field->cell[4] == field->cell[2] && field->cell[4] == field->cell[6]) ||
            (field->cell[4] == field->cell[3] && field->cell[4] == field->cell[5]))
        {
            return (field->cell[4] == cell_player1) ? fs_end_win_p1 : fs_end_win_p2;
        }
    }
    if (field->cell[8])
    {
        if ((field->cell[8] == field->cell[6] && field->cell[8] == field->cell[7]) ||
            (field->cell[8] == field->cell[2] && field->cell[8] == field->cell[5]))
        {
            return (field->cell[8] == cell_player1) ? fs_end_win_p1 : fs_end_win_p2;
        }
    }
    return (field->empty_cell_count) ? fs_in_progress : fs_end_draw;
}