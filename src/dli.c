/*
  Firefighter Display List Interrupt routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2023-12-24
*/

#include "dli.h"

extern unsigned char font1_data[];

void dli(void) {
  asm("pha");

  asm("sta $D40A"); // WSYNC

  /* Flip between the game character sets */
  asm("lda $14"); // <== RTCLOK (lowest)
  asm("lsr"); // Slow it down
  asm("lsr");
  asm("and #$04"); // Truncate all but "4" bit
  asm("adc $600"); // Add the character set base (is there a better way of doing this? -bjk 2023.08.13)
  asm("sta $D409"); // ==> CHBASE

  /* Different color palette for the game area */
  asm("lda $D20A"); // <== RANDOM
  asm("and #$0F"); // Truncate high bits
  asm("asl");
  //asm("adc #$20"); // Add yellow
  asm("adc $602"); // Add yellow
  asm("sta $D016"); // ==> COLOR0

  //asm("lda #$CA"); // <== Medium green
  asm("lda $603"); // <== Medium green
  asm("sta $D017"); // COLOR1

  //asm("lda #$86"); // <== Medium blue
  asm("lda $604"); // <== Medium blue
  asm("sta $D018"); // ==> COLOR2

  asm("lda $601"); // <== PEEK($601)
  asm("sta $D01A"); // ==> COLOR4 (background)

  asm("pla");
  asm("rti");
}
