/*
  Firefighter game loop and its helper functions.

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-15 - 2023-08-27
*/

#ifndef _GAME_H
#define _GAME_H

/* Level size/shape constants */
#define LEVEL_W 20
#define LEVEL_H 11
#define LEVEL_SPAN (LEVEL_W * LEVEL_H)
#define LEVEL_TOT_SIZE (LEVEL_SPAN + 2)

void start_game(void);

#endif

