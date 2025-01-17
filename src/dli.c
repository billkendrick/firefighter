/*
  Firefighter Display List Interrupt routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2025-01-17
*/

#include <atari.h>
#include "dli.h"

void * OLDVEC;

void dli2(void);

void dli(void) {
  asm("pha");

  asm("sta %w", (unsigned)&ANTIC.wsync);

  /* Flip between the game character sets */
  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("lsr"); // Slow it down
  asm("lsr");
  asm("and $606"); // Truncate all but "4" bit (0x04 in normal situations)
  asm("adc $600"); // Add the character set base (is there a better way of doing this? -bjk 2023.08.13)
  asm("adc #2");
  asm("sta %w", (unsigned)&ANTIC.chbase);

  /* Different color palette for the game area */
  asm("lda %w", (unsigned)&POKEY_READ.random);
  asm("and $605"); // Truncate high bits (0x0F in normal situations)
  asm("asl");
  asm("adc $602"); // Add yellow
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf0);

  asm("lda $603"); // <== Medium green
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);

  asm("lda $604"); // <== Medium blue
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf2);

  asm("lda $601"); // <== Background color (black)
  asm("sta %w", (unsigned)&GTIA_WRITE.colbk);

  asm("lda # <%v", dli2);
  asm("sta %w", (unsigned)&OS.vdslst);
  asm("lda # >%v", dli2);
  asm("sta %w", ((unsigned)&OS.vdslst) + 1);

  asm("pla");
  asm("rti");
}

void dli2(void) {
  asm("pha");
  asm("txa");
  asm("pha");

  /* --- */
  asm("sta %w", (unsigned)&ANTIC.wsync);

  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("lsr"); // Slow it down
  asm("lsr");
  asm("and $606"); // Truncate all but "4" bit (0x04 in normal situations)
  asm("adc $600"); // Add the character set base (is there a better way of doing this? -bjk 2023.08.13)
  asm("adc #2");
  asm("sta %w", (unsigned)&ANTIC.chbase);

  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("lsr");
  asm("and $605"); // Truncate high bits (0x0F in normal situations)
  asm("adc $602"); // Add yellow
  asm("tax");

  /* --- */
  asm("stx %w", (unsigned)&GTIA_WRITE.colpf0);
  asm("inx");
  asm("inx");
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.wsync);

  asm("stx %w", (unsigned)&GTIA_WRITE.colpf0);
  asm("inx");
  asm("inx");
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.wsync);

  asm("stx %w", (unsigned)&GTIA_WRITE.colpf0);
  asm("inx");
  asm("inx");
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.wsync);

  asm("stx %w", (unsigned)&GTIA_WRITE.colpf0);
  asm("inx");
  asm("inx");
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.wsync);

  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("lsr"); // Slow it down
  asm("lsr");
  asm("and $606"); // Truncate all but "4" bit (0x04 in normal situations)
  asm("adc $600"); // Add the character set base (is there a better way of doing this? -bjk 2023.08.13)
  asm("adc #2");
  asm("sta %w", (unsigned)&ANTIC.chbase);

  asm("pla");
  asm("tax");
  asm("pla");

  asm("rti");
}

void dli_vbi(void) {
  asm("lda # <%v", dli);
  asm("sta %w", (unsigned)&OS.vdslst);
  asm("lda # >%v", dli);
  asm("sta %w", ((unsigned)&OS.vdslst) + 1);

  asm("jmp (%v)", OLDVEC);
}

