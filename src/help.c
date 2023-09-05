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

#define FANCY_HELP_IO

#ifdef FANCY_HELP_IO
#define LINES 22
#define MAX_PAGES 50 // Should be plenty? Check ATASCII "README.txt"'s output length, divided by LINES
#else
#define LINES 23
#endif

/* Controls/commands within the help screen */
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
unsigned char ciov(void);
unsigned char xio_open_read(char * filespec);
unsigned char xio_close(void);
unsigned char xio_note(unsigned char * ptr);
unsigned char xio_point(unsigned char * ptr);
unsigned char xio_get_record(char * buf, unsigned int buf_size, unsigned int * read_len);
#endif

/* Routine to load and show help text on a fullscreen text display */
void show_help(void) {
  int i, j;
  unsigned char y, last, cmd, eof;
  char str[41];
#ifdef FANCY_HELP_IO
  int cur_page;
  unsigned char err;
  unsigned int len;
  unsigned char ptr[3];
  unsigned char ptrs[MAX_PAGES * 3];
#else
  FILE * fi;
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

  /* Open the help text file for read */
#ifdef FANCY_HELP_IO
  cur_page = 0;
  err = xio_open_read("D:README.TXT");
  if (err > 127)
    return;
#else
  fi = fopen("README.TXT", "r");
  if (fi == NULL)
    return;
#endif


  /* -- Main help screen loop -- */
  do {
#ifdef FANCY_HELP_IO
    err = xio_note(ptr);
    ptrs[cur_page * 3 + 0] = ptr[0];
    ptrs[cur_page * 3 + 1] = ptr[1];
    ptrs[cur_page * 3 + 2] = ptr[2];
/*
    draw_text("NOTED: ", scr_mem + (LINES * 40) + 8);
    draw_number(ptr[0], 3, scr_mem + (LINES * 40) + 20);
    draw_number(ptr[1], 3, scr_mem + (LINES * 40) + 24);
    draw_number(ptr[2], 3, scr_mem + (LINES * 40) + 28);
    draw_number(err, 3, scr_mem + (LINES * 40) + 37);
*/
#endif

    /* Display a screenful (page) of text */
    y = 0;
    eof = 0;

    do {
#ifdef FANCY_HELP_IO
      err = xio_get_record(str, sizeof(str), &len);
      str[len] = '\0';

      if (err == 136)
        eof = 1;
#else
      fgets(str, sizeof(str), fi);
      eof = feof(fi);
#endif

      last = strlen(str) - 1;
      if (str[last] == 0x9B /* ATASCII EOL */) {
        str[last] = '\0';
      }
      draw_text(str, scr_mem + y * 40);
      y++;

    } while (!eof && y < LINES);

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
        if (!eof) {
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
      xio_point(ptrs + (cur_page * 3));
    }
#endif
  } while (!eof && cmd != HELP_CMD_EXIT);

#ifdef FANCY_HELP_IO
  xio_close();
#else
  fclose(fi);
#endif

  /* (Eat input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;
}

#ifdef FANCY_HELP_IO

#define HELP_TEXT_IOCB 2
#define IOCB_OPEN_READ 4

/* Open a file for read via CIO routine.
   @param char * filespec file to open (e.g., "D:FILE.TXT")
   @return unsigned char CIO status code
*/
unsigned char xio_open_read(char * filespec) {
  OS.iocb[HELP_TEXT_IOCB].command = IOCB_OPEN;
  OS.iocb[HELP_TEXT_IOCB].buffer = filespec;
  OS.iocb[HELP_TEXT_IOCB].buflen = strlen(filespec);
  OS.iocb[HELP_TEXT_IOCB].aux1 = IOCB_OPEN_READ;
  OS.iocb[HELP_TEXT_IOCB].aux2 = 0;
  return ciov();
}

/* Close the file opened by `xio_open_read()` (via CIO routine).
   @return unsigned char CIO status code
*/
unsigned char xio_close(void) {
  OS.iocb[HELP_TEXT_IOCB].command = IOCB_CLOSE;
  return ciov();
}

/* Read a record (line of text, up to ATASCII EOF character)
   from the file opened by `xio_open_read()` (via CIO routine).
   @param char * buf buffer to store the results
   @param unsigned int buf_size maximum amount of data `buf` can accept
   @param unsigned int * read_line where we return the size of data actually loaded
   @return unsigned char CIO status code
*/
unsigned char xio_get_record(char * buf, unsigned int buf_size, unsigned int * read_len) {
  unsigned char err;

  OS.iocb[HELP_TEXT_IOCB].command = IOCB_GETREC;
  OS.iocb[HELP_TEXT_IOCB].buffer = buf;
  OS.iocb[HELP_TEXT_IOCB].buflen = buf_size;
  err = ciov();

  if (err > 127) {
    *read_len = 0;
  } else {
    *read_len = OS.iocb[HELP_TEXT_IOCB].buflen;
  }
  return err;
}

/* Grab a pointer of where we were in the file opened by
   `xio_open_read()` (via CIO routine).
   @param unsigned char ptr[3] buffer to store the pointer (send it to `xio_point()`, below)
   @return unsigned char CIO status code
*/
unsigned char xio_note(unsigned char ptr[3]) {
  OS.iocb[HELP_TEXT_IOCB].command = IOCB_NOTE;
  OS.iocb[HELP_TEXT_IOCB].buffer = ptr;
  OS.iocb[HELP_TEXT_IOCB].buflen = sizeof(ptr);
  return ciov();
}

/* Point to a position in the file opened by `xio_open_read()` (via CIO routine).
   @param unsigned char ptr[3] buffer storing the pointer (previously read by `xio_note()`, above)
   @return unsigned char CIO status code
*/
unsigned char xio_point(unsigned char ptr[3]) {
  OS.iocb[HELP_TEXT_IOCB].command = IOCB_POINT;
  OS.iocb[HELP_TEXT_IOCB].buffer = ptr;
  OS.iocb[HELP_TEXT_IOCB].buflen = sizeof(ptr);
  return ciov();
}

/* Call OS's CIO vector routine.
   @return unsigned char CIO status code
*/
unsigned char ciov(void) {
  asm("LDX #%b", HELP_TEXT_IOCB << 4);
  asm("JSR $E456");
  asm("TYA"); /* Get the error code */
  /* This is probably a dumb way of returning */
  asm("STA $6FF");
  return (PEEK(0x6ff));
}
#endif

