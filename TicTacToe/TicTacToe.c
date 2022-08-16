//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#include "player.h"
#include "game.h"

#include "utils.h"

//================== DECLARATION ===================//

static Player       main_player;
static Key          menu_choice;

static void     t3Logo                      (void);
static void     t3MenuMain                  (void);
static void     t3MenuSingleplayer          (void);
static void     t3MenuHotsit                (void);
static void     t3MenuCredits               (void);
static void     t3MenuExit                  (void);

//====================== MAIN ======================//
int main(int argc, char **argv)
{
    if (!t3OSPrepareIO())
		return EXIT_FAILURE;
	
	//t3ScreenClear();
    t3Logo();
	t3MenuMain();
    
	return EXIT_SUCCESS;
}

//================= IMPLEMENTATION =================//

void t3Logo(void)
{
    puts("| x | o | x | o | x | o | x |");
    puts("+---+                   +---+");
    puts("| o |    Tic-Tac-Toe    | o |");
    puts("+---+                   +---+");
    puts("| x | o | x | o | x | o | x |");

    t3Waiting(5);
    t3ScreenClear();
}

void t3MenuMain(void)
{
    while (1)
    {
        t3ScreenClear();
        puts("+------------------------------+");
        puts("|           MAIN MENU          |");
        puts("+------------------------------+");
        puts("| 1. Singleplayer              |");
        puts("| 2. Hotsit                    |");
        puts("| 3. Credits                   |");
        puts("| 4. Exit                      |");
        puts("+------------------------------+");

        menu_choice = t3InputGet(T3_NULLPTR);
        
        switch (menu_choice)
        {
        case T3_KEY_NUM_1: t3MenuSingleplayer();         break;
        case T3_KEY_NUM_2: t3MenuHotsit();               break;
        case T3_KEY_NUM_3: t3MenuCredits();              break;
        case T3_KEY_NUM_4: return;
        }
    }
}

void t3MenuSingleplayer(void)
{
    while (1)
    {
        t3ScreenClear();

        puts("+------------------------------+");
        puts("|         SINGLEPLAYER         |");
        puts("+------------------------------+");
        puts("| Select AI difficult          |");
        puts("| 1. Easy                      |");
        puts("| 2. Normal                    |");
        puts("| 3. Hard                      |");
        puts("| 4. Return to main menu       |");
        puts("+------------------------------+");

        menu_choice = t3InputGet(T3_NULLPTR);
        
        if (T3_KEY_NUM_4 == menu_choice)
            return;
        else
        {
            Player second_player;
            switch (menu_choice)
            {
            case T3_KEY_NUM_1: t3PlayerFactory(&second_player, AI_EASY); break;
            case T3_KEY_NUM_2: t3PlayerFactory(&second_player, AI_NORM); break;
            case T3_KEY_NUM_3: t3PlayerFactory(&second_player, AI_HARD); break;
            }
            t3Game(&main_player, &second_player);
        }
    }
}

void t3MenuHotsit(void)
{
    t3ScreenClear();

    puts("+------------------------------+");
    puts("|            HOTSIT            |");
    puts("+------------------------------+");

    Player second_player;

    t3PlayerFactory(&second_player, HUMAN);

    t3Game(&main_player, &second_player);
}

void t3MenuCredits(void)
{
    t3ScreenClear();
    puts("| x | o | x | o | x | o | x | o | x | o | x | o | x | o | x | o | x |");
    puts("+---+                                                           +---+");
    puts("| o |                          CREDITS                          | o |");
    puts("+---+                                                           +---+");
    puts("| x |    Thank you for playing in my version of Tic-Tac-Toe!    | x |");
    puts("+---+           You are free to modify it in anyway.            +---+");
    puts("| o |                   Game has MIT License.                   | o |");
    puts("+---+          Check other my games on gitlab or site.          +---+");
    puts("| x |          Gitlab: https://gitlab.com/oleg_oleynik          | x |");
    puts("+---+            Site: https://cosmoshedgehog.com/              +---+");
    puts("| o |   Thanks my family and friends for testing and helping.   | o |");
    puts("+---+                  Special thanks: vladchaos                +---+");
    puts("| x |           Thank my love Katrin for her support :-*        | x |");
    puts("+---+                                                           +---+");
    puts("| o | x | o | x | o | x | o | x | o | x | o | x | o | x | o | x | o |");
    
    t3InputGet("Press any key to continue...");
}