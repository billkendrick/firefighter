/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-08-22
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "game.h"

extern unsigned char scr_mem[];
unsigned char * dlist = (scr_mem + 512);
unsigned long int high_score;
char high_score_name[4];
char main_stick;
char level;

void main(void) {
  high_score = 1031;
  strcpy(high_score_name, "BJK");
  main_stick = STICK_LEFT;
  level = 1;

  do {
    show_title();
    start_game();
  } while(1);
}
