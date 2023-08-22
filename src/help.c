/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-22 - 2023-08-22
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdio.h>
#include "help.h"

extern unsigned char scr_mem[];
extern unsigned char * dlist;

/* FIXME */
void show_help(void) {
  int i;

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);
  POKE(dlist + 6, DL_BLK2);

  for (i = 7; i < 30; i++) {
    POKE(dlist + i, DL_GRAPHICS0);
  }

  POKE(dlist + 31, DL_JVB);
  POKEW(dlist + 32, (unsigned int) dlist);

  OS.sdlst = dlist;

  OS.color0 = 0x52;
  OS.color1 = 0xCA;
  OS.color2 = 0x02;
  OS.color3 = 0x46;
  OS.color4 = 0x02;

  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);

  do {
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0);

  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
}
