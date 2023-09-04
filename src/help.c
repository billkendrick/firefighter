/*
  Firefighter Help Text display

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-22 - 2023-09-04
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdio.h>
#include "help.h"
#include "draw_text.h"

#define LINES 22

enum {
  HELP_CMD_NONE,
  HELP_CMD_NEXT,
  HELP_CMD_PREV,
  HELP_CMD_EXIT,
};

extern unsigned char scr_mem[];
extern unsigned char * dlist;

/* Routine to load and show help text on a fullscreen text display */
void show_help(void) {
  int i, j;
  unsigned char y, last, cmd;
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

  for (i = 6; i < 6 + LINES - 1; i++) {
    POKE(dlist + i, DL_GRAPHICS0);
  }

  POKE(dlist + i++, DL_BLK4);

  for (j = LINES; j < 24; j++)
    POKE(dlist + i++, DL_GRAPHICS0);

  POKE(dlist + i++, DL_JVB);
  POKEW(dlist + i++, (unsigned int) dlist);

  OS.sdlst = dlist;

  OS.color1 = 0x0F;
  OS.color2 = 0x44;
  OS.color4 = 0x42;

  OS.chbas = 0xE0; // OS ROM default character set

  OS.sdmctl = (DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH);

  draw_text("SPACE/RETURN/FIRE: Next Page - ESC: Exit", scr_mem + (LINES * 40));
  draw_text("UP/BACKSPACE: Previous Page", scr_mem + ((LINES + 1) * 40) + 7);

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
      } while (!feof(fi) && y < LINES);

      /* (Eat input) */
      do {
      } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
      OS.ch = KEY_NONE;

      /* Wait for input */
      cmd = HELP_CMD_NONE;
      do {
        if (OS.strig0 == 0 || OS.strig1 == 0 ||
            OS.ch == KEY_SPACE || OS.ch == KEY_RETURN) {
          cmd = HELP_CMD_NEXT;
        }
        if (CONSOL_START(GTIA_READ.consol) == 1 || OS.ch == KEY_ESC) {
          cmd = HELP_CMD_EXIT;
        }
        if (0) { // FIXME
          cmd = HELP_CMD_PREV;
        }

      } while (cmd == HELP_CMD_NONE);

      bzero(scr_mem, 40 * LINES);
    } while (!feof(fi) && cmd != HELP_CMD_EXIT);
    fclose(fi);
  }

  /* (Eat input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;
}

