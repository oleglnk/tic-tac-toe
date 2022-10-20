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

#define MOVE_AI_HUMAN    cell_player1
#define MOVE_AI          cell_player2

typedef struct Move_tag
{
    int score;
    uint index;
} Move;

typedef void(*MoveCompareAI)(Move * best_move, Move const * move, Cell const move_cur);

static MoveCompareAI compare;
static Field         ai_field;
static uint          empty_cell_count;

static void t3MoveCompareNormAI(
    Move       * best_move,
    Move const * move,
    Cell const   move_cur)
{
    if (move->score > best_move->score)
    {
        best_move->score = move->score;
        best_move->index = move->index;
    }
}

static void t3MoveCompareHardAI(
    Move       * best_move,
    Move const * move,
    Cell const   move_cur)
{
    if (move_cur == MOVE_AI)
    {
        if (move->score > best_move->score)
        {
            best_move->score = move->score;
            best_move->index = move->index;
        }
    }
    else
    {
        if (move->score < best_move->score)
        {
            best_move->score = move->score;
            best_move->index = move->index;
        }
    }
}

static Move t3MiniMax(
    Field       * field,
    Cell  const   move_cur)
{
    Move move, best_move;
    best_move.score = (move_cur == MOVE_AI) ? -10000 : 10000;
    int i = 0;
    best_move.index = 0;
    while (i < FIELD_SIZE)
    {
        if (cell_empty == field->cell[i])
        {
            field->empty_cell_count--;
            field->cell[i] = move_cur;

            switch (t3FieldGetStatus(field))
            {
            case fs_in_progress:
                move = t3MiniMax(field, (MOVE_AI_HUMAN == move_cur) ? MOVE_AI : MOVE_AI_HUMAN);
                break;
            case fs_end_win_p1: move.score = -1; break;
            case fs_end_win_p2: move.score = +1; break;
            default:            move.score =  0; break;
            }

            field->cell[i] = cell_empty;
            empty_cell_count++;

            compare(&best_move, &move, move_cur);
        }
        i++;
    }
    return best_move;
}

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

static uint t3TurnAiNorm(
    Field const * field)
{
    compare = t3MoveCompareNormAI;
    t3FieldCopy(&ai_field, field);
    return t3MiniMax(&ai_field, MOVE_AI).index;
}

static uint t3TurnAiHard(
    Field const * field)
{
    compare = t3MoveCompareHardAI;
    t3FieldCopy(&ai_field, field);
    return t3MiniMax(&ai_field, MOVE_AI).index;
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