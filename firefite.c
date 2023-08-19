/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-13
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"

void main(void) {
  int i;
  do {
    show_title();
  } while(1);
}
