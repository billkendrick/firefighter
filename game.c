/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-15 - 2023-08-15
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "game.h"
#include "shapes.h"
#include "draw_text.h"
#include "dli.h"

extern unsigned char font1_data[];
// extern unsigned char font2_data[]; /* Not actually referenced */

extern unsigned char scr_mem[];
extern unsigned char * dlist;

void start_game(void) {
  int i;

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);
  POKE(dlist + 6, DL_BLK1);

  POKE(dlist + 7, DL_GRAPHICS2);

  POKE(dlist + 8, DL_DLI(DL_BLK1));

  for (i = 9; i < 19; i++) {
    POKE(dlist + i, DL_GRAPHICS2);
  }

  POKE(dlist + 20, DL_JVB);
  POKEW(dlist + 21, (unsigned int) dlist);

  OS.sdlst = dlist;
  OS.chbas = (unsigned char) ((unsigned int) font1_data / 256);

  POKE(0x600, OS.chbas + 2);

  OS.color0 = 0x52;

  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;

  draw_text("NEW BREED SOFTWARE PRESENTS:", scr_mem + 6);
  draw_text("@ FIREFIGHTER! @", scr_mem + 40 + 2);

  OS.sdmctl = 34;

  do {
  } while (OS.strig0 == 1);

  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;
}
