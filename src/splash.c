/*
  Firefighter splash screen app.

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  https://www.newbreedsoftware.com/firefighter/

  2023-09-19 - 2026-10-12
*/

#include <atari.h>
#include <peekpoke.h>
#include <stdio.h>
#include <string.h>
#include "rmt-c.h"

/* Display list (at the 1K boundary), followed by
   the first chunk of the graphics data up until the 4K boundary,
   then the second chunk of graphics data */
extern unsigned char scr_mem[];
unsigned char * dlist = scr_mem;
unsigned char * scr_mem_pt1 = (scr_mem + 0x150);
unsigned char * scr_mem_pt2 = (scr_mem + 0x1000);
unsigned char * scr_mem_row[191];
unsigned char * scr_mem_row_src1, * scr_mem_row_src2, * scr_mem_row_dest;
unsigned char * pmg = (scr_mem + 0x2000);

void eat_input(void);

char loading_str[] = "LOADING FIREFIGHTER...";

/* The song data (RMT -> ASM via dmsc's mkreloc) */
extern char rmt_song_data[];


/* Main loop! */
void main(void) {
  FILE * fi;
  int ply, i, x, l;

  rmt_init(rmt_song_data, 2 /* FIXME: ??? */);

  OS.sdmctl = 0;
  while (ANTIC.vcount < 124);
  OS.gprior = PRIOR_GFX_MODE_9; // 0b01000000;
  OS.color4 = 0x30; // dark orangish-red

  /* Set up display list */

  /* (Start by filling with all GRAPHICS 8 instructions;
     other necessary things will be added directly below.
     Unlike other parts of the game, we're not recording
     the whole Display List as an array) */
  memset(dlist + 6, DL_GRAPHICS8, 192);

  POKE(dlist + 0, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS8));
  POKEW(dlist + 4, (unsigned int) scr_mem_pt1);

  /* ... */

  POKE(dlist + 99, DL_LMS(DL_GRAPHICS8));
  POKEW(dlist + 100, (unsigned int) scr_mem_pt2);

  /* ... */

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

#define SKIP 48
#define PMG_TOP 32
#define PMG_LEFT 48

  /* Load the silhouette */
  bzero(pmg, 2048);
  fi = fopen("silh.dat", "rb");
  if (fi != NULL) {
    for (ply = 0; ply < 5; ply++) {
      unsigned char * ptr;
      ptr = (unsigned char *) ((unsigned int) pmg + ((unsigned int) ply * 256) + 768);
      fread(ptr + ((unsigned char) (PMG_TOP + SKIP)), sizeof(unsigned char), 192 - SKIP, fi);
    }
    fclose(fi);

    GTIA_WRITE.hposm3 = PMG_LEFT + 0;
    GTIA_WRITE.hposm2 = PMG_LEFT + 4;
    GTIA_WRITE.hposm1 = PMG_LEFT + 8;
    GTIA_WRITE.hposm0 = PMG_LEFT + 12;
    GTIA_WRITE.hposp0 = PMG_LEFT + 16;
    GTIA_WRITE.hposp1 = PMG_LEFT + 32;
    GTIA_WRITE.hposp2 = PMG_LEFT + 48;
    GTIA_WRITE.hposp3 = PMG_LEFT + 64;

    GTIA_WRITE.sizep0 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep1 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep2 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizep3 = PMG_SIZE_DOUBLE;
    GTIA_WRITE.sizem = (
      (PMG_SIZE_DOUBLE << 6) |
      (PMG_SIZE_DOUBLE << 4) |
      (PMG_SIZE_DOUBLE << 2) |
      (PMG_SIZE_DOUBLE << 0)
    );

    OS.pcolr0 = 0xF0;
    OS.pcolr1 = 0xF0;
    OS.pcolr2 = 0xF0;
    OS.pcolr3 = 0xF0;
    ANTIC.pmbase = (unsigned char) ((unsigned int) pmg / 256);
    GTIA_WRITE.gractl = GRACTL_MISSLES | GRACTL_PLAYERS;
  }

  /* Show the screen, start the music, and wait for input before proceeding */
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH | DMACTL_DMA_MISSILES | DMACTL_DMA_PLAYERS | DMACTL_PMG_SINGLELINE;

  rmt_start();

  eat_input();
  do {
  } while (OS.strig0 == 1 && OS.strig1 == 1 && OS.ch == 255 && GTIA_READ.consol == 7);

  for (i = 0; i < 16; i++) {
      rmt_global_volume_fade = i << 4;
      OS.color4 = i;
      OS.pcolr0 = i;
      OS.pcolr1 = i;
      OS.pcolr2 = i;
      OS.pcolr3 = i;
      rmt_wait_vbi();
  }

  rmt_stop();

  eat_input();


  /* Unset GRAPHICS 9 GTIA mode, move the player/missiles out of the way */
  OS.sdmctl = 0;
  while (ANTIC.vcount < 124);
  OS.gprior = 0;

  GTIA_WRITE.gractl = 0;
  GTIA_WRITE.hposm3 = 0;
  GTIA_WRITE.hposm2 = 0;
  GTIA_WRITE.hposm1 = 0;
  GTIA_WRITE.hposm0 = 0;
  GTIA_WRITE.hposp0 = 0;
  GTIA_WRITE.hposp1 = 0;
  GTIA_WRITE.hposp2 = 0;
  GTIA_WRITE.hposp3 = 0;


  /* Set up a mostly-off (no DMA fetch) screen, with a
     single line of text in the middle, to show a "loading"
     message. */
  memset((unsigned char *) 0x600, DL_BLK8, 13);

  POKE(0x600 + 12, DL_LMS(DL_GRAPHICS0));
  POKEW(0x600 + 13, (unsigned int) 0x680);

  POKE(0x600 + 15, DL_JVB);
  POKEW(0x600 + 16, (unsigned int) dlist);

  l = strlen(loading_str);
  x = (32 - l) / 2;
  for (i = 0; i < l; i++) {
    POKE(0x680 + x + i, loading_str[i] - 32);
  }

  OS.color2 = 0x40;
  OS.color1 = 0x0F;

  OS.sdlst = (unsigned char *) 0x600;

  OS.sdmctl = DMACTL_PLAYFIELD_NARROW | DMACTL_DMA_FETCH;

  /* ...MyDOS will load the game itself, as autorun file .AR1 */
}

void eat_input(void) {
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || GTIA_READ.consol != 7);
  OS.ch = 255;
}

