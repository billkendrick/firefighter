/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-20
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

void main(void) {
  high_score = 123456;
  strcpy(high_score_name, "BJK");

  do {
    show_title();
    start_game();
  } while(1);
}
