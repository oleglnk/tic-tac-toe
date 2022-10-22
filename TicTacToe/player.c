//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#include "player.h"

#include "io.h"

#include <string.h>
//TODO: Delete in C23
#include <stdbool.h>

//=============    PRIVATE SECTION    ==============//

static Field         ai_field;

static uint t3TurnPlayer(
    Field const * field)
{
    Key key;
    while (1)
    {
        key = t3GetKey(nullptr);
        if (t3_key_None == key)
        {
            puts("Wrong cell number! Please try again.");
        }
        else
        {
            uint index = (uint)key - (uint)t3_key_num1;
            if (field->cell[index] != cell_empty)
                puts("Cell is already occupied! Please try again.");
            else
                return index;
        }
    }
}

static uint t3TurnAiEasy(
    Field const * field)
{
    int max_chances = 0;
    uint index = FIELD_SIZE;
    for (uint i = 0; i < FIELD_SIZE; i++)
    {
        if (field->cell[i] == cell_empty)
        {
            int chances = rand();
            if ( (index == FIELD_SIZE) ||
                ((index >= 0) && (max_chances < chances)))
            {
                index = i;
                max_chances = chances;
            }
        }
    }
    return index;
}

static int t3MiniMax(
    Field       * field,
    bool  const   isMaximizing)
{
    switch (t3FieldGetStatus(field))
    {
    case fs_end_win_p1: return -1;
    case fs_end_win_p2: return +1;
    case fs_end_draw:   return 0;
    }

    int best_score = isMaximizing ? -10000 : 10000;
    for (uint i = 0; i < FIELD_SIZE; i++)
    {
        if (cell_empty == field->cell[i])
        {
            field->cell[i] = isMaximizing ? cell_player2 : cell_player1;
            field->empty_cell_count--;

            int score = t3MiniMax(field, !isMaximizing);

            field->cell[i] = cell_empty;
            field->empty_cell_count++;

            if (isMaximizing)
                best_score = (score > best_score) ? score : best_score;
            else
                best_score = (score < best_score) ? score : best_score;
        }
    }
    return best_score;
}

static uint t3TurnAiNorm(
    Field const * field)
{
    t3FieldCopy(&ai_field, field);
    int best_score = -10000;
    uint best_index;
    int max_chances = -1;
    for (uint i = 0; i < FIELD_SIZE; i++)
    {
        if (cell_empty == ai_field.cell[i])
        {
            ai_field.cell[i] = cell_player2;
            ai_field.empty_cell_count--;

            int score = t3MiniMax(&ai_field, false);
            int chances = rand();

            ai_field.cell[i] = cell_empty;
            ai_field.empty_cell_count++;
            
            if ((score > best_score) && 
                (max_chances < chances))
            {
                best_score = score;
                best_index = i;
                max_chances = chances;
            }
        }
    }
    return best_index;
}

static uint t3TurnAiHard(
    Field const * field)
{
    t3FieldCopy(&ai_field, field);
    int best_score = -10000;
    uint best_index;
    for (uint i = 0; i < FIELD_SIZE; i++)
    {
        if (cell_empty == ai_field.cell[i])
        {
            ai_field.cell[i] = cell_player2;
            ai_field.empty_cell_count--;

            int score = t3MiniMax(&ai_field, false);

            ai_field.cell[i] = cell_empty;
            ai_field.empty_cell_count++;
            
            if (score > best_score)
            {
                best_score = score;
                best_index = i;
            }
        }
    }
    return best_index;
}

static void t3RemoveSpaces(char * src)
{
    char* temp = src;
    do
    {
        while ((*temp == ' ') || (*temp == '\n'))
            ++temp;
    } while (*src++ = *temp++);
}

static void t3NameGet(
    char       * name,
    bool const   auto_select)
{
    if (auto_select)
    {
        static char const * names[] = {
            "John", "Bob",
            "Jessica", "Alice"};
        size_t i = rand() % (sizeof(names) / sizeof(names[0]));
        strncpy(name, names[i], PLAYERNAME_SIZE_MAX);
    }
    else
    {
        puts("Please enter player's name (20 characters maximum).");
        while (1)
        {
            fgets(name, FILENAME_MAX, stdin);
            t3RemoveSpaces(name);
            if (name[0])
                break;
            else
                puts("Incorrect input!");
        }
    }
}


//==============    PUBLIC SECTION    ==============//

void t3PlayerFactory(Player * player, PlayerType const type)
{
    t3NameGet(player->name, !(type == pl_human));
    switch (type)
    {
    case pl_human:   player->turn = t3TurnPlayer; break;
    case pl_ai_easy: player->turn = t3TurnAiEasy; break;
    case pl_ai_norm: player->turn = t3TurnAiNorm; break;
    case pl_ai_hard: player->turn = t3TurnAiHard; break;
    }
}