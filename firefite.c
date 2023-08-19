/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-15
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "game.h"

extern unsigned char scr_mem[];
unsigned char * dlist = (scr_mem + 512);

void main(void) {
  do {
    show_title();
    start_game();
  } while(1);
}
