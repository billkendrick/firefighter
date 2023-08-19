/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-14
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include "title.h"
#include "shapes.h"
#include "draw_text.h"
#include "dli.h"

extern unsigned char font1_data[];
// extern unsigned char font2_data[]; /* Not actually referenced */

extern unsigned char scr_mem[];
unsigned char * dlist = (scr_mem + 512);

void show_title(void) {
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

  POKE(dlist + 7, DL_GRAPHICS2);
  POKE(dlist + 8, DL_BLK2);
  POKE(dlist + 9, DL_GRAPHICS1);

  POKE(dlist + 10, DL_DLI(DL_BLK8));

  for (i = 0; i < 3; i++) {
    POKE(dlist + 11 + i * 4, DL_GRAPHICS0);
    POKE(dlist + 11 + i * 4 + 1, DL_BLK2);
    POKE(dlist + 11 + i * 4 + 2, DL_GRAPHICS2);
    POKE(dlist + 11 + i * 4 + 3, DL_BLK6);
  }

  POKE(dlist + 26, DL_BLK2);
  for (i = 27; i < 32; i++)
    POKE(dlist + i, DL_GRAPHICS0);

  POKE(dlist + 32, DL_JVB);
  POKEW(dlist + 33, (unsigned int) dlist);

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
  draw_text("bill kendrick", scr_mem + 60 + 1);
  POKE(scr_mem + 75, 2 + 16 + 64);
  POKE(scr_mem + 76, 0 + 16 + 64);
  POKE(scr_mem + 77, 2 + 16 + 64);
  POKE(scr_mem + 78, 3 + 16 + 64);

  draw_text("GAS PIPE  LEAKING GAS  TURN VALVE", scr_mem + 80 + 3);
  POKE(scr_mem + 122, PIPE_DOWN_RIGHT);
  POKE(scr_mem + 123, PIPE_LEFT_RIGHT);
  POKE(scr_mem + 124, PIPE_UP_LEFT);

  POKE(scr_mem + 128, PIPE_BROKEN_UP_DOWN);
  POKE(scr_mem + 129, GASLEAK_RIGHT);

  POKE(scr_mem + 134, PIPE_LEFT_RIGHT);
  POKE(scr_mem + 135, VALVE_OPEN);
  POKE(scr_mem + 136, PIPE_LEFT_RIGHT);

  draw_text("GET AX  BREAK CRATES   OIL    WALLS", scr_mem + 140 + 2);
  POKE(scr_mem + 182, AX);

  POKE(scr_mem + 187, CRATE);
  POKE(scr_mem + 188, CRATE_BROKEN);

  POKE(scr_mem + 193, OIL);

  POKE(scr_mem + 196, WALL);
  POKE(scr_mem + 197, WALL);
  POKE(scr_mem + 198, WALL);


  draw_text("PUT OUT FIRE, LEAD CIVILIANS TO EXIT", scr_mem + 200 + 2);
  POKE(scr_mem + 240, FIRE_LG);
  POKE(scr_mem + 241, FIRE_MD);
  POKE(scr_mem + 242, FIRE_SM);

  POKE(scr_mem + 243, 'e' + 64);
  POKE(scr_mem + 244, 'g' + 64);
  POKE(scr_mem + 245, FIREMAN_LEFT);

  POKE(scr_mem + 248, CIVILIAN);
  POKE(scr_mem + 249, CIVILIAN);
  POKE(scr_mem + 250, FIREMAN_RIGHT);

  POKE(scr_mem + 255, DOOR);
  POKE(scr_mem + 257, EXIT1);
  POKE(scr_mem + 258, EXIT2);

  draw_text("LEFT STICK MOVES, FIRE CHANGES TOOL.", scr_mem + 260 + 2);
  draw_text("RIGHT STICK USES TOOL IN THAT DIRECTION.", scr_mem + 300);
  // draw_text("", scr_mem + 340);
  draw_text("PRESS START OR FIRE TO BEGIN!!", scr_mem + 380 + 5);
  draw_text("INSPIRED BY \"REAL HEROES: FIREFIGHTER\"", scr_mem + 420 + 1);

  OS.sdmctl = 34;

  do {
    OS.color0 = OS.rtclok[2];
  } while (OS.strig0 == 1 && CONSOL_START(GTIA_READ.consol) == 0);

  OS.sdmctl = 0;
  ANTIC.nmien = NMIEN_VBI;
}
