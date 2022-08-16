//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#include "field.h"

#define T3_TIPS_ENABLED

void	t3FieldInit(
	Field			  * field,
	char		const   symbol_player_1, 
	char		const   symbol_player_2)
{
    for (int i = 0; i < FIELD_SIZE; i++)
        field->cell[i] = EMPTY;
    
    field->empty_cell_count = FIELD_SIZE;

    field->symbol_player_1  = symbol_player_1;
    field->symbol_player_2  = symbol_player_2;
}

void	t3FieldCopy(
	Field			  * dest,
	Field		const * src)
{
    for (int i = 0; i < FIELD_SIZE; i++)
        dest->cell[i] = src->cell[i];

    dest->empty_cell_count = src->empty_cell_count;

    dest->symbol_player_1 = src->symbol_player_1;
    dest->symbol_player_2 = src->symbol_player_2;
}

void	t3FieldShow(
	Field		const * field)
{
    for (int i = 0; i < FIELD_SIZE; i++)
    {
        putchar(' ');
        
        switch (field->cell[i])
        {
        case EMPTY:    putchar(' '); break;
        case PLAYER_1: putchar(field->symbol_player_1); break;
        case PLAYER_2: putchar(field->symbol_player_2); break;
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
#ifdef T3_TIPS_ENABLED
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

FieldStatus     t3FieldGetStatus(
	Field		const * field)
{
    if (field->cell[0])
    {
        if ((field->cell[0] == field->cell[1] && field->cell[0] == field->cell[2]) ||
            (field->cell[0] == field->cell[3] && field->cell[0] == field->cell[6]))
        {
            return (field->cell[0] == PLAYER_1) ? END_WIN_P1 : END_WIN_P2;
        }
    }
    if (field->cell[4])
    {
        if ((field->cell[4] == field->cell[0] && field->cell[4] == field->cell[8]) ||
            (field->cell[4] == field->cell[1] && field->cell[4] == field->cell[7]) ||
            (field->cell[4] == field->cell[2] && field->cell[4] == field->cell[6]) ||
            (field->cell[4] == field->cell[3] && field->cell[4] == field->cell[5]))
        {
            return (field->cell[4] == PLAYER_1) ? END_WIN_P1 : END_WIN_P2;
        }
    }
    if (field->cell[8] != EMPTY)
    {
        if ((field->cell[8] == field->cell[6] && field->cell[8] == field->cell[7]) ||
            (field->cell[8] == field->cell[2] && field->cell[8] == field->cell[5]))
        {
            return (field->cell[8] == PLAYER_1) ? END_WIN_P1 : END_WIN_P2;
        }
    }
    return (0 == field->empty_cell_count) ? END_DRAW : IN_PROGRESS;
}