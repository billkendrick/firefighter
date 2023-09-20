/*
  Firefighter splash screen app.

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-09-19 - 2023-09-19
*/

#include <atari.h>
#include <peekpoke.h>
#include <stdio.h>
#include <string.h>

/* Display list (at the 1K boundary), followed by
   the first chunk of the graphics data up until the 4K boundary,
   then the second chunk of graphics data */
extern unsigned char scr_mem[];
unsigned char * dlist = scr_mem;
unsigned char * scr_mem_pt1 = (scr_mem + 0x150);
unsigned char * scr_mem_pt2 = (scr_mem + 0x1000);

void eat_input(void);


/* Main loop! */
void main(void) {
  FILE * fi;
  unsigned char i;

  OS.sdmctl = 0;
  OS.gprior = 0b01000000;
  OS.color4 = 0x30;

  /* Set up display list */
  POKE(dlist + 0, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);
  POKE(dlist + 3, DL_LMS(DL_GRAPHICS8));
  POKEW(dlist + 4, (unsigned int) scr_mem_pt1);
  for (i = 6; i < 99; i++) {
    POKE(dlist + i, DL_GRAPHICS8);
  }
  POKE(dlist + 99, DL_LMS(DL_GRAPHICS8));
  POKEW(dlist + 100, (unsigned int) scr_mem_pt2);
  for (i = 102; i < 198; i++) {
    POKE(dlist + i, DL_GRAPHICS8);
  }
  POKE(dlist + 199, DL_JVB);
  POKEW(dlist + 200, (unsigned int) dlist);

  OS.sdlst = dlist;

  /* Load the image */
  /* TODO It could be baked into the XEX file! */
  fi = fopen("title.gr9", "rb");
  if (fi != NULL) {
    fread(scr_mem_pt1, sizeof(unsigned char), 7680, fi);
    fclose(fi);
  }

  /* Show the screen and wait for input before proceeding */
  OS.sdmctl = 34;

  eat_input();
  do {
  } while (OS.strig0 == 1 && OS.strig1 == 1 && OS.ch == 255 && GTIA_READ.consol == 7);
  eat_input();

  /* Blank the screen (and unset GRAPHICS 9 ANTIC mode), and exit */
  OS.sdmctl = 0;
  OS.gprior = 0;

  /* ...MyDOS will load the game itself, as autorun file .AR1 */
}

void eat_input(void) {
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || GTIA_READ.consol != 7);
  OS.ch = 255;
}

