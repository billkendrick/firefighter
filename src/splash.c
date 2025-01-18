/*
  Firefighter splash screen app.

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-09-19 - 2025-01-17
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
unsigned char * scr_mem_row[191];
unsigned char * scr_mem_row_src1, * scr_mem_row_src2, * scr_mem_row_dest;
unsigned char * pmg = (scr_mem + 0x2000);

void eat_input(void);

//#define FIRE_OUTRO

#ifdef FIRE_OUTRO
unsigned char fade[61] = {
  0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,8,8,
  8,8,8,9,9,9,9,9,10,10,10,10,10,11,11,11,11,11,12,12,12
};

unsigned char fade_shift4[61] = {
  0,0,0,16,16,16,16,16,32,32,32,32,32,48,48,48,48,48,64,64,64,64,64,80,80,80,80,80,
  96,96,96,96,96,112,112,112,112,112,128,128,128,128,128,144,144,144,144,144,160,
  160,160,160,160,176,176,176,176,176,192,192,192
};
#pragma bss-name (push,"ZEROPAGE")
  unsigned char x, y, i;
  unsigned char c, c1, c2, c3, c4;
  unsigned int new_c;
#pragma bss-name (pop)
#endif


/* Main loop! */
void main(void) {
  FILE * fi;
  int ply;

  OS.sdmctl = 0;
  while (ANTIC.vcount < 124);
  OS.gprior = 0b01000000;
  OS.color4 = 0x30;

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

  /* Show the screen and wait for input before proceeding */
  OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH | DMACTL_DMA_MISSILES | DMACTL_DMA_PLAYERS | DMACTL_PMG_SINGLELINE;

  eat_input();
  do {
  } while (OS.strig0 == 1 && OS.strig1 == 1 && OS.ch == 255 && GTIA_READ.consol == 7);


/* Fire effect; very slow in straight C, at the moment */

#ifdef FIRE_OUTRO
  for (y = 0; y < 191; y++) {
    scr_mem_row[y] = scr_mem_pt1 + (y * 40);
  }

/*
xx T1 T2 xx
S1 S2 S3 S4
xx S5 S6 xx

T1 = S1, S2, S3, S5
T2 = S2, S3, S4, S6
*/

  for (i = 0; i < 8; i++) {
    for (y = 0; y < 191; y++) {
      scr_mem_row_dest = scr_mem_row[y];
      scr_mem_row_src1 = scr_mem_row[y + 1];
      scr_mem_row_src2 = scr_mem_row[y + 2];

      for (x = 0; x < 40; x++) {
        c1 = PEEK(scr_mem_row_src1 + x);
#define        s1  ((c1 & 0xF0) >> 4)
#define        s2  ((c1 & 0x0F))

        c2 = PEEK(scr_mem_row_src1 + x + 1);
#define        s3  ((c2 & 0xF0) >> 4)
#define        s4  ((c2 & 0x0F))

        c3 = PEEK(scr_mem_row_src2 + x);
#define        s5  ((c3 & 0x0F))

        c4 = PEEK(scr_mem_row_src2 + x + 1);
#define        s6  ((c4 & 0xF0) >> 4)

        new_c = fade[(s1 + s2 + s3 + s5)];
        c = PEEK(scr_mem_row_dest + x) & 0xF0;
        POKE(scr_mem_row_dest + x, c | new_c);

        new_c = fade_shift4[(s2 + s3 + s4 + s6)];
        c = PEEK(scr_mem_row_dest + x + 1) & 0x0F;
        POKE(scr_mem_row_dest + x + 1, c | new_c);
      }
    }
  }
#endif


  eat_input();

  /* Blank the screen (and unset GRAPHICS 9 ANTIC mode), and exit */
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

  /* ...MyDOS will load the game itself, as autorun file .AR1 */
}

void eat_input(void) {
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || GTIA_READ.consol != 7);
  OS.ch = 255;
}

