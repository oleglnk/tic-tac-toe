//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#include "player.h"

#include "field.h"

#include "utils.h"

#include <string.h>

//==================================================//
//                                                  //
//               DECLARATION SECTION                //
//                                                  //
//==================================================//

#define T3_MOVE_AI_HUMAN    PLAYER_1
#define T3_MOVE_AI          PLAYER_2
#define T3_PLAYER_FILENAME  "default.t3"

typedef struct  t3_tag_Move                 Move;

typedef void(*t3MoveCompareAI)(Move * best_move, Move const * move, Cell move_cur);

enum t3_tag_PLAYER_RATING
{
    RATE_HUMAN   = 1000,
    RATE_AI_EASY = 1000,
    RATE_AI_NORM = 2000,
    RATE_AI_HARD = 2500,
};

struct t3_tag_Move
{
    int score;
    int index;
};

static t3MoveCompareAI                      compare;
static Field                                ai_field;
static int                                  empty_cell_count;

static int      t3TurnPlayer                (Field const * field);
static int      t3TurnAiEasy                (Field const * field);
static int      t3TurnAiNorm                (Field const * field);
static int      t3TurnAiHard                (Field const * field);

static Move     t3MiniMax                   (Field       * field, Cell move_cur);

static void     t3MoveCompareNormAI         (Move * best_move, Move const * move, Cell move_cur);
static void     t3MoveCompareHardAI         (Move * best_move, Move const * move, Cell move_cur);

static char*    t3RemoveSpaces              (char * s);
static void     t3NameGet                   (char * name, int auto_select);



void            t3PlayerFactory             (Player * player, PlayerType type)
{
    char name[T3_PLAYER_NAME_SIZE_MAX];
    if (type == HUMAN)
        t3NameGet(name, false);
    else
        t3NameGet(name, true);
    switch (type)
    {
    case HUMAN:
        player->mmr  = RATE_HUMAN;
        player->turn = t3TurnPlayer;
        strncpy(player->name, name, T3_PLAYER_NAME_SIZE_MAX);
        break;
    case AI_EASY:
        player->mmr  = RATE_AI_EASY;
        player->turn = t3TurnAiEasy;
        strncpy(player->name, name, T3_PLAYER_NAME_SIZE_MAX);
        break;
    case AI_NORM:
        player->mmr  = RATE_AI_NORM;
        player->turn = t3TurnAiNorm;
        strncpy(player->name, name, T3_PLAYER_NAME_SIZE_MAX);
        break;
    case AI_HARD:
        player->mmr  = RATE_AI_HARD;
        player->turn = t3TurnAiHard;
        strncpy(player->name, name, T3_PLAYER_NAME_SIZE_MAX);
        break;
    }
}
bool            t3PlayerDelete              (Player * player)
{
    return (!remove(T3_PLAYER_FILENAME));
}

bool            t3PlayerLoad                (Player * player)
{
    FILE *pFile = fopen(T3_PLAYER_FILENAME, "rb");
    bool success = false;
    if (pFile)
    {
        char buffer[2];
        fread(buffer, sizeof(char), 2, pFile);

        if (('T' == buffer[0]) && ('3' == buffer[1]))
        {
            fread(player->name, sizeof(player->name[0]), T3_PLAYER_NAME_SIZE_MAX, pFile);
            fread(&player->mmr, sizeof(player->mmr), 1, pFile);
            
            success = true;
        }
        fclose(pFile);
    }
    return success;
}

void      t3PlayerSave                      (Player const * player)
{
    FILE *pFile = fopen(T3_PLAYER_FILENAME, "wb");
    if (pFile)
    {
        fwrite("T3", sizeof(char), 2, pFile);
        fwrite(player->name, sizeof(player->name[0]), T3_PLAYER_NAME_SIZE_MAX, pFile);
        fwrite(&player->mmr, sizeof(player->mmr), 1, pFile);
        fclose(pFile);
    }
}

//==================================================//
//                                                  //
//                  PRIVATE SECTION                 //
//                                                  //
//==================================================//

int t3TurnPlayer(Field const * field)
{
    Key key;
    while (1)
    {
        key = t3InputGet(T3_NULLPTR);
        if (T3_KEY_UNDEFINED == key)
        {
            puts("Wrong cell number! Please try again.");
        }
        else
        {
            int index = (int)key - (int)T3_KEY_NUM_1;
            if (field->cell[index] != EMPTY)
                puts("Cell is already occupied! Please try again.");
            else
                return index;
        }
    }
}

int             t3TurnAiEasy                (Field const * field)
{
    static int empty_cells[FIELD_SIZE];
    int i;
    empty_cell_count = 0;
    for (i = 0; i < FIELD_SIZE; i++)
    {
        if (field->cell[i] == EMPTY)
        {
            empty_cells[empty_cell_count] = i;
            empty_cell_count++;
        }
    }
    i = t3RandomInt(0, empty_cell_count - 1);
    return empty_cells[i];
}

int             t3TurnAiNorm                (Field const * field)
{
    compare = t3MoveCompareNormAI;
    t3FieldCopy(&ai_field, field);
    return t3MiniMax(&ai_field, T3_MOVE_AI).index;
}

int             t3TurnAiHard                (Field const * field)
{
    compare = t3MoveCompareHardAI;
    t3FieldCopy(&ai_field, field);
    return t3MiniMax(&ai_field, T3_MOVE_AI).index;
}

void            t3MoveCompareNormAI         (Move * best_move, Move const * move, Cell move_cur)
{
    if (move->score > best_move->score)
    {
        best_move->score = move->score;
        best_move->index = move->index;
    }
}

void            t3MoveCompareHardAI         (Move * best_move, Move const * move, Cell move_cur)
{
    if (move_cur == T3_MOVE_AI)
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

Move            t3MiniMax                   (Field * field, Cell move_cur)
{
    Move move, best_move;
    best_move.score = (move_cur == T3_MOVE_AI) ? -10000 : 10000;
    int i = 0;
    best_move.index = 0;
    while (i < FIELD_SIZE)
    {
        if (EMPTY == field->cell[i])
        {
            field->empty_cell_count--;
            field->cell[i] = move_cur;

            switch (t3FieldGetStatus(field))
            {
            case IN_PROGRESS:
                move = t3MiniMax(field, (T3_MOVE_AI_HUMAN == move_cur) ? T3_MOVE_AI : T3_MOVE_AI_HUMAN);
                break;
            case END_WIN_P1: move.score = -1; break;
            case END_WIN_P2: move.score = +1; break;
            default:         move.score =  0; break;
            }

            field->cell[i] = EMPTY;
            empty_cell_count++;

            compare(&best_move, &move, move_cur);
        }
        i++;
    }
    return best_move;
}

char*           t3RemoveSpaces              (char * s)
{
    char* d = s;
    do
    {
        while (*d == ' ')
            ++d;
    } while (*s++ = *d++);
    return s;
}

void            t3NameGet                  (char * name, int auto_select)
{
    if (auto_select)
    {
        static char const * names[] = {
            "John", "Bob",
            "Jessica", "Alice"};
        int i = t3RandomInt(0, 4);
        strncpy(name, names[i], T3_PLAYER_NAME_SIZE_MAX);
    }
    else
    {
        puts("Please enter player's name (20 characters maximum).");
        size_t read_count;
        while (1)
        {
            read_count = fread(name, sizeof(char), T3_PLAYER_NAME_SIZE_MAX, stdin);
            name[read_count - 1] = '\0';
            if (t3RemoveSpaces(name))
                break;
            else
                puts("Incorrect input!");
        }
    }
}