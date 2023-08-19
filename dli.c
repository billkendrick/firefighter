/*
  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/

  2023-08-13 - 2023-08-14
*/

#include "dli.h"

extern unsigned char font1_data[];
// extern unsigned char font2_data[]; /* Not actually referenced */

void dli(void) {
  asm("pha");

  asm("sta $D40A"); // WSYNC

  /* Different color palette for the game area */
  asm("lda #$02"); // <== Dark grey
  asm("sta $D01A"); // ==> COLOR4 (background)

  asm("lda #$C8"); // <== Medium green
  asm("sta $D017");

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
