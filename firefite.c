/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-13
*/

#include <atari.h>
#include <peekpoke.h>
#include <string.h>

extern unsigned char font1_data[];
extern unsigned char font2_data[];

extern unsigned char scr_mem[];
unsigned char * dlist = (scr_mem + 512);

void draw_text(char * str, unsigned char * dest);
void dli(void);

void main(void) {
  int i;

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

  for (i = 0; i < 5; i++) {
    POKE(dlist + 11 + i * 4, DL_GRAPHICS0);
    POKE(dlist + 11 + i * 4 + 1, DL_BLK2);
    POKE(dlist + 11 + i * 4 + 2, DL_GRAPHICS2);
    POKE(dlist + 11 + i * 4 + 3, DL_BLK6);
  }

  POKE(dlist + 11 + i * 4, DL_JVB);
  POKEW(dlist + 11 + i * 4 + 1, (unsigned int) dlist);

  OS.sdlst = dlist;
  OS.chbas = (unsigned char) ((unsigned int) font1_data / 256);

  POKE(0x600, OS.chbas + 2);

  ANTIC.nmien = NMIEN_VBI;
  while (ANTIC.vcount < 124);
  OS.vdslst = (void *) dli;
  ANTIC.nmien = NMIEN_VBI | NMIEN_DLI;

  draw_text("NEW BREED SOFTWARE PRESENTS:", scr_mem + 6);
  draw_text("@ FIREFIGHT @", scr_mem + 40 + 3);
  draw_text("bill kendrick 2023", scr_mem + 60 + 1);

#define PIPE_UP_DOWN (1 + 64)
#define PIPE_BROKEN_UP_DOWN (2 + 64)
#define PIPE_LEFT_RIGHT (3 + 64)
#define PIPE_BROKEN_LEFT_RIGHT (4 + 64)

#define PIPE_DOWN_RIGHT (5 + 64)
#define PIPE_DOWN_LEFT (6 + 64)
#define PIPE_UP_LEFT (7 + 64)
#define PIPE_UP_RIGHT (8 + 64)

#define VALVE_OPEN (9 + 64)
#define VALVE_CLOSED (10 + 64)

#define CIVILIAN (11 + 64)

#define GASLEAK_RIGHT 12
#define GASLEAK_LEFT 13
#define GASLEAK_DOWN 14
#define GASLEAK_UP 15

#define OIL (16 + 64)

#define CRATE (17 + 192)
#define CRATE_BROKEN (18 + 192)

#define FIREMAN_LEFT (19 + 128)
#define FIREMAN_RIGHT (20 + 128)

#define AX (21 + 64)

#define FIRE_SM 22
#define FIRE_MD 23
#define FIRE_LG 24

#define DOOR (26 + 192)

#define EXIT1 (27 + 192)
#define EXIT2 (28 + 192)

#define WALL (25 + 192)

  draw_text("GAS PIPE (LEAK) CRATES    AX  OIL  WALL", scr_mem + 80);
  POKE(scr_mem + 120, PIPE_DOWN_RIGHT);
  POKE(scr_mem + 121, PIPE_LEFT_RIGHT);
  POKE(scr_mem + 122, VALVE_OPEN);
  POKE(scr_mem + 123, PIPE_UP_LEFT);

  POKE(scr_mem + 125, PIPE_BROKEN_UP_DOWN);
  POKE(scr_mem + 126, GASLEAK_RIGHT);

  POKE(scr_mem + 128, CRATE);
  POKE(scr_mem + 130, CRATE_BROKEN);

  POKE(scr_mem + 133, AX);

  POKE(scr_mem + 135, OIL);

  POKE(scr_mem + 137, WALL);
  POKE(scr_mem + 138, WALL);
  POKE(scr_mem + 139, WALL);

  draw_text("PUT OUT FIRE, LEAD CIVILIANS TO EXIT", scr_mem + 140 + 2);
  POKE(scr_mem + 180, FIRE_LG);
  POKE(scr_mem + 181, FIRE_MD);
  POKE(scr_mem + 182, FIRE_SM);

  POKE(scr_mem + 183, 'e' + 64);
  POKE(scr_mem + 184, 'g' + 64);
  POKE(scr_mem + 185, FIREMAN_LEFT);

  POKE(scr_mem + 189, CIVILIAN);
  POKE(scr_mem + 190, CIVILIAN);
  POKE(scr_mem + 191, FIREMAN_RIGHT);

  POKE(scr_mem + 195, DOOR);
  POKE(scr_mem + 197, EXIT1);
  POKE(scr_mem + 198, EXIT2);

  draw_text("0123456789012345678901234567890123456789", scr_mem + 200);
//  draw_text("ABCDEFGHIJABCDEFGHIJ", scr_mem + 240);

  draw_text("0123456789012345678901234567890123456789", scr_mem + 260);
//  draw_text("ABCDEFGHIJABCDEFGHIJ", scr_mem + 300);

  draw_text("0123456789012345678901234567890123456789", scr_mem + 320);
//  draw_text("ABCDEFGHIJABCDEFGHIJ", scr_mem + 360);

  do {} while(1);
}

void dli(void) {
  asm("pha");

  asm("sta $D40A"); // WSYNC

  /* Different color palette for the game area */
  asm("lda #$02"); // <== Dark grey
  asm("sta $D01A"); // ==> COLOR4 (background)

  asm("lda $D20A"); // <== RANDOM
  asm("and #$0F"); // Truncate high bits
  asm("asl");
  asm("adc #$20"); // Add yellow
  asm("sta $D016"); // ==> COLOR0

  /* Flip between the game character sets */
  asm("lda $14"); // <== RTCLOK (lowest)
  asm("lsr"); // Slow it down
  asm("lsr");
  asm("and #$04"); // Truncate all but "4" bit
  asm("adc $600"); // Add the character set base (is there a better way of doing this? -bjk 2023.08.13)
  asm("sta $D409"); // ==> CHBASE

  asm("pla");
  asm("rti");
}

void draw_text(char * str, unsigned char * dest) {
  unsigned char ch;
  unsigned int i;

  for (i = 0; str[i] != '\0'; i++) {
    ch = str[i];

    if (ch < 32) {
      ch = ch + 64;
    } else if (ch < 96) {
      ch = ch - 32;
    }

    POKE((unsigned int) (dest + i), ch);
  }
}

