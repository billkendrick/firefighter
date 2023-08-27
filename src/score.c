/*
  Firefighter scoring & high score functions

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-27 - 2023-08-27
*/

#include <string.h>
#include "score.h"

unsigned long int high_score;
char high_score_name[4];

void set_default_high_score(void) {
  high_score = 1031;
  strcpy(high_score_name, "BJK");
}
