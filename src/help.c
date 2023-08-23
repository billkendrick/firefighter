/*
  Firefighter Help Text display

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
#include "draw_text.h"

extern unsigned char scr_mem[];
extern unsigned char * dlist;

/* Routine to load and show help text on a fullscreen text display */
void show_help(void) {
  int i;
  unsigned char y, last;
  FILE * fi;
  char str[41];

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);

  for (i = 6; i < 28; i++) {
    POKE(dlist + i, DL_GRAPHICS0);
  }

  POKE(dlist + 28, DL_BLK1);
  POKE(dlist + 29, DL_GRAPHICS0);

  POKE(dlist + 30, DL_JVB);
  POKEW(dlist + 31, (unsigned int) dlist);

  OS.sdlst = dlist;

  OS.color1 = 0x0F;
  OS.color2 = 0x44;
  OS.color4 = 0x42;

  OS.chbas = 0xE0; // OS ROM default character set

  OS.sdmctl = (DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH);

  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);

  draw_text("SPACE/FIRE: Continue - ESCAPE: Exit", scr_mem + (23 * 40) + 2);

  fi = fopen("README.TXT", "r");
  if (fi != NULL) {
    do {
      y = 0;
      do {
        fgets(str, sizeof(str), fi);
        last = strlen(str) - 1;
        if (str[last] == 0x9B /* ATASCII EOL */) {
          str[last] = '\0';
        }
        draw_text(str, scr_mem + y * 40);
        y++;
      } while (!feof(fi) && y < 23);

      OS.ch = KEY_NONE;
      do {
      } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0 && OS.ch != KEY_SPACE && OS.ch != KEY_ESC);
  
      bzero(scr_mem, 40 * 23);
    } while (!feof(fi) && CONSOL_START(GTIA_READ.consol) == 0 && OS.ch != KEY_ESC);
    fclose(fi);
  }

  OS.ch = KEY_NONE;

  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
}

