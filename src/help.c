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

#ifdef FANCY_HELP_IO
#define LINES 22
#define MAX_PAGES 50 // Should be plenty? Check ATASCII "README.txt"'s output length, divided by LINES
#else
#define LINES 23
#endif

enum {
  HELP_CMD_NONE,
  HELP_CMD_NEXT,
#ifdef FANCY_HELP_IO
  HELP_CMD_PREV,
#endif
  HELP_CMD_EXIT,
};

extern unsigned char scr_mem[];
extern unsigned char * dlist;

/* Local function prototypes: */
#ifdef FANCY_HELP_IO
void ciov(void);
void note(unsigned char * ptr);
void point(unsigned char * ptr);
#endif

/* Routine to load and show help text on a fullscreen text display */
void show_help(void) {
  int i, j;
  unsigned char y, last, cmd;
  FILE * fi;
  char str[41];
#ifdef FANCY_HELP_IO
  int cur_page;
  unsigned char ptrs[MAX_PAGES * 3];
#endif

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

  /* Show control instructions */
  draw_text("SPACE/RETURN/FIRE: Next Page - ESC: Exit", scr_mem + (LINES * 40));
#ifdef FANCY_HELP_IO
  draw_text("UP/BACKSPACE: Previous Page", scr_mem + ((LINES + 1) * 40) + 7);
#endif

  fi = fopen("README.TXT", "r");
  if (fi != NULL) {
#ifdef FANCY_HELP_IO
    cur_page = 0;
#endif

    /* -- Main help screen loop -- */
    do {
#ifdef FANCY_HELP_IO
      note(ptrs + (cur_page * 3));
#endif

      /* Display a screenful (page) of text */
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

#ifdef FANCY_HELP_IO
      cur_page++;
      draw_number(cur_page, 2, scr_mem + ((LINES + 1) * 40));
#endif

      /* (Eat input) */
      do {
      } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
      OS.ch = KEY_NONE;

      /* Wait for input */
      cmd = HELP_CMD_NONE;
      do {
        if (OS.strig0 == 0 || OS.strig1 == 0 ||
            OS.ch == KEY_SPACE || OS.ch == KEY_RETURN) {
          /* Show next page (or exit, if we're at the end) */
          if (!feof(fi)) {
            cmd = HELP_CMD_NEXT;
          } else {
            cmd = HELP_CMD_EXIT;
          }
        }

        if (CONSOL_START(GTIA_READ.consol) == 1 || OS.ch == KEY_ESC) {
          /* Exit immediately */
          cmd = HELP_CMD_EXIT;
        }

#ifdef FANCY_HELP_IO
        if (OS.stick0 == 14 || OS.stick1 == 14 || OS.ch == KEY_DASH || OS.ch == (KEY_DASH | KEY_CTRL)) {
          /* Go back a page */
          if (cur_page > 1) {
            cmd = HELP_CMD_PREV;
          }
        }
#endif
      } while (cmd == HELP_CMD_NONE);

      bzero(scr_mem, 40 * LINES);


#ifdef FANCY_HELP_IO
      if (cmd == HELP_CMD_PREV) {
        /* Go back a page */
        cur_page -= 2;
        point(ptrs + (cur_page * 3));
      }
#endif

    } while (!feof(fi) && cmd != HELP_CMD_EXIT);
    fclose(fi);
  }

  /* (Eat input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;
}

#ifdef FANCY_HELP_IO
void note(unsigned char * ptr) {
  OS.iocb[0].command = IOCB_NOTE;
  OS.iocb[0].buffer = ptr;
  OS.iocb[0].buflen = 3;
  ciov();
}

void point(unsigned char * ptr) {
  OS.iocb[0].command = IOCB_POINT;
  OS.iocb[0].buffer = ptr;
  OS.iocb[0].buflen = 3;
  ciov();
}

/*
cio.s
        ;; Call CIO

        .export _ciov

_ciov:  LDX #$00
        JSR $E456
        RTS
*/
void ciov(void) {
  asm("LDX #$00"); // IOCB #
  asm("JSR $E456");
}
#endif

