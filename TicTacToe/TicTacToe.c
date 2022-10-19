//==================================================
//
// Copyright © 2021-2022 Oleg Oleynik
// License: MIT License (X11 License)
//
//==================================================

#include "player.h"
#include "game.h"

#include "io.h"

#include <time.h>

//===============    DECLARATION    ================//

static Player       main_player;
static Key          menu_choice;

static void t3Logo(float const seconds)
{
    puts("| x | o | x | o | x | o | x |");
    puts("+---+                   +---+");
    puts("| o |    Tic-Tac-Toe    | o |");
    puts("+---+                   +---+");
    puts("| x | o | x | o | x | o | x |");

    //Waiting to continue
    clock_t delay = (clock_t)(seconds * CLOCKS_PER_SEC);  // convert to clock ticks
    clock_t start = clock();
    while (clock() - start < delay)  // wait until time elapses
        ;                            // note the semicolon
    t3ScreenClear();
}

static void t3MenuVersusAI(void)
{
    while (1)
    {
        t3ScreenClear();

        puts("+------------------------------+");
        puts("|         Versus A.I.          |");
        puts("+------------------------------+");
        puts("| Select AI difficult          |");
        puts("| 1. Easy                      |");
        puts("| 2. Normal                    |");
        puts("| 3. Hard                      |");
        puts("| 4. Return to main menu       |");
        puts("+------------------------------+");

        menu_choice = t3GetKey(nullptr);

        if (t3_key_num4 == menu_choice)
            return;
        else
        {
            Player second_player;
            switch (menu_choice)
            {
            case t3_key_num1: t3PlayerFactory(&second_player, pl_ai_easy); break;
            case t3_key_num2: t3PlayerFactory(&second_player, pl_ai_norm); break;
            case t3_key_num3: t3PlayerFactory(&second_player, pl_ai_hard); break;
            }
            t3Game(&main_player, &second_player);
        }
    }
}

static void t3MenuHotsit(void)
{
    t3ScreenClear();

    puts("+------------------------------+");
    puts("|            Hotsit            |");
    puts("+------------------------------+");

    Player second_player;

    t3PlayerFactory(&second_player, pl_human);

    t3Game(&main_player, &second_player);
}

static void t3MenuCredits(void)
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
    puts("| x |          Gitlab: https://gitlab.com/oleglnk               | x |");
    puts("+---+            Site: https://oleglnk.pages.dev/               +---+");
    puts("| o |   Thanks my family and friends for testing and helping.   | o |");
    puts("+---+                  Special thanks: vladchaos                +---+");
    puts("| x |           Thank my love Katrin for her support :-*        | x |");
    puts("+---+                                                           +---+");
    puts("| o | x | o | x | o | x | o | x | o | x | o | x | o | x | o | x | o |");

    t3GetKey("Press any key to continue...");
}

static void t3MenuMain(void)
{
    while (1)
    {
        t3ScreenClear();
        puts("+------------------------------+");
        puts("|           Main Menu          |");
        puts("+------------------------------+");
        puts("| 1. Versus A.I.               |");
        puts("| 2. Hotsit                    |");
        puts("| 3. Credits                   |");
        puts("| 4. Exit                      |");
        puts("+------------------------------+");

        menu_choice = t3GetKey(nullptr);

        switch (menu_choice)
        {
        case t3_key_num1: t3MenuVersusAI(); break;
        case t3_key_num2: t3MenuHotsit();   break;
        case t3_key_num3: t3MenuCredits();  break;
        case t3_key_num4: return;
        }
    }
}



//===================    MAIN    ===================//

int main(int argc, char ** argv)
{
    if (!t3IOInit())
		return EXIT_FAILURE;
	
	//t3ScreenClear();
    t3Logo(5.0f);
	t3MenuMain();
    
	return EXIT_SUCCESS;
}