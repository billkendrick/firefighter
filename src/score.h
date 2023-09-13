/*
  Firefighter scoring & high score functions

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-27 - 2023-09-13
*/

#ifndef _SCORE_H
#define _SCORE_H

/* NOTE: If any of these are changed, update "Scoring" section of README.md! */

#define SCORE_AX_COLLECT 15
#define SCORE_CIVILIAN_RESCUE 100
#define SCORE_CRATE_BREAK_DEDUCTION 1
#define SCORE_PIPE_BREAK_DEDUCTION 5
#define SCORE_NO_FIRE_BONUS 1000

void set_default_high_score(void);
char register_high_score(void);

#ifdef DISK
void load_high_scores(void);
void save_high_scores(void);
void show_high_score_table(char highlight);
#endif

#endif
