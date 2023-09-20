/*
  Firefighter Help Text display

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-22 - 2023-09-20
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>
#include <stdio.h>
#include "help.h"
#include "draw_text.h"

#define LINES 22
#define MAX_PAGES 50 // Should be plenty? Check ATASCII "README.txt"'s output length, divided by LINES

/* Controls/commands within the help screen */
#define HELP_CMD_NONE 0
#define HELP_CMD_NEXT 1
#define HELP_CMD_PREV 2
#define HELP_CMD_EXIT 255

extern unsigned char scr_mem[];
extern unsigned char * dlist;

/* Local function prototypes: */
void show_help_controls(unsigned char first_page, unsigned char last_page);
unsigned char ciov(void);
unsigned char xio_open_read(char * filespec);
unsigned char xio_close(void);
unsigned char xio_note(unsigned char * ptr);
unsigned char xio_point(unsigned char * ptr);
unsigned char xio_get_record(char * buf, unsigned int buf_size, unsigned int * read_len);


/* Routine to load and show help text on a fullscreen text display */
void show_help(void) {
  unsigned char y, last, cmd, eof;
  char str[41];
  unsigned char cur_page;
  unsigned char err;
  unsigned int len;
  unsigned char ptr[3];
  unsigned char ptrs[MAX_PAGES * 3];

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */

  /* (Start by filling with all GRAPHICS 0 instructions;
     other necessary things will be added directly below.
     Unlike other parts of the game, we're not recording
     the whole Display List as an array) */
  memset(dlist + 6, DL_GRAPHICS0, 24);

  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);

  /* ... */

  POKE(dlist + 6 + LINES - 1, DL_BLK4);

  /* ... */

  POKE(dlist + 30, DL_JVB);
  POKEW(dlist + 31, (unsigned int) dlist);

  OS.sdlst = dlist;

  OS.color1 = 0x0F;
  OS.color2 = 0x44;
  OS.color4 = 0x42;

  OS.chbas = 0xE0; // OS ROM default character set

  OS.sdmctl = (DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH);

  /* Show control instructions (the first time) */
  show_help_controls(1, 0);

  /* Open the help text file for read */
  cur_page = 0;
  err = xio_open_read("D:README.TXT");
  if (err > 127)
    return;


  /* -- Main help screen loop -- */
  do {
    err = xio_note(ptr);
    ptrs[cur_page * 3 + 0] = ptr[0];
    ptrs[cur_page * 3 + 1] = ptr[1];
    ptrs[cur_page * 3 + 2] = ptr[2];

    /* Display a screenful (page) of text */
    y = 0;
    eof = 0;

    do {
      err = xio_get_record(str, sizeof(str), &len);
      str[len] = '\0';

      if (err == 136)
        eof = 1;

      last = strlen(str) - 1;
      if (str[last] == 0x9B /* ATASCII EOL */) {
        str[last] = '\0';
      }
      draw_text(str, scr_mem + y * 40 + 1); /* N.B. Indent by 1 to center it; text is word-wrapped at 38 characters */
      y++;

    } while (!eof && y < LINES);

    cur_page++;
    draw_number(cur_page, 2, scr_mem + ((LINES + 1) * 40));

    show_help_controls((cur_page == 1), eof);

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

      if (OS.stick0 == 14 || OS.stick1 == 14 || OS.ch == KEY_DASH || OS.ch == (KEY_DASH | KEY_CTRL)) {
        /* Go back a page */
        if (cur_page > 1) {
          cmd = HELP_CMD_PREV;
        }
      }
    } while (cmd == HELP_CMD_NONE);

    bzero(scr_mem, 40 * LINES);


    if (cmd == HELP_CMD_PREV) {
      /* Go back a page */
      cur_page -= 2;
      xio_point(ptrs + (cur_page * 3));
    }
  } while (cmd != HELP_CMD_EXIT);

  xio_close();

  /* (Eat input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;
}

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
  unsigned char err;

  OS.iocb[HELP_TEXT_IOCB].command = IOCB_NOTE;
  err = ciov();

  /* h/t https://www.atariarchives.org/mmm/iocbs.php */
  ptr[0] = OS.iocb[HELP_TEXT_IOCB].aux3;
  ptr[1] = OS.iocb[HELP_TEXT_IOCB].aux4;
  ptr[2] = OS.iocb[HELP_TEXT_IOCB].aux5;

  return err;
}

/* Point to a position in the file opened by `xio_open_read()` (via CIO routine).
   @param unsigned char ptr[3] buffer storing the pointer (previously read by `xio_note()`, above)
   @return unsigned char CIO status code
*/
unsigned char xio_point(unsigned char ptr[3]) {
  OS.iocb[HELP_TEXT_IOCB].command = IOCB_POINT;

  /* h/t https://www.atariarchives.org/mmm/iocbs.php */
  OS.iocb[HELP_TEXT_IOCB].aux3 = ptr[0];
  OS.iocb[HELP_TEXT_IOCB].aux4 = ptr[1];
  OS.iocb[HELP_TEXT_IOCB].aux5 = ptr[2];

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


/* Show help screen controls at the bottom of the screen.
   @param unsigned char first_page true, if we're on the first page
   @param unsigned char last_page true, if we're on the last page (have hit EOF)
*/
void show_help_controls(unsigned char first_page, unsigned char last_page) {
  if (!last_page) {
    draw_text("SPACE/RETURN/FIRE: Next Page - ESC: Exit", scr_mem + (LINES * 40));
  } else {
    draw_text("      SPACE/RETURN/FIRE/ESC: Exit       ", scr_mem + (LINES * 40));
  }

  if (!first_page) {
    draw_text("UP/BACKSPACE: Previous Page", scr_mem + ((LINES + 1) * 40) + 7);
  } else {
    draw_text("                           ", scr_mem + ((LINES + 1) * 40) + 7);
  }
}

