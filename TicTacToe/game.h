//==================================================//
//   Copyright © 2021 Oleg Oleynik                  //
//   Author:  Oleg Oleynik                          //
//   License: MIT License (X11 License)             //
//==================================================//

#ifndef H_GAME_T3_INCLUDED_
#define H_GAME_T3_INCLUDED_

/*  operation:      create & process the game       */
/*  preconditions:  players' data                   */
/*  postconditions: changed MMR of each player      */
/*                  updated leaderboard             */
void            t3Game                      (Player * player1, Player * player2);

#endif