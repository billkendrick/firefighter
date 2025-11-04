/*
  Firefighter Display List Interrupt routines

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-13 - 2025-11-04
*/

#include <atari.h>
#include "dli.h"

void * OLDVEC;

void dli1(void);
void dli2(void);

void dli(void) {
  asm("pha");

  asm("lda $607");
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.chbase);

  /* Different color palette for the game area */
  asm("lda $603"); // <== Medium green
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);

  asm("lda $604"); // <== Medium blue
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf2);

  asm("lda $601"); // <== Background color (black)
  asm("sta %w", (unsigned)&GTIA_WRITE.colbk);

  asm("lda # <%v", dli1);
  asm("sta %w", (unsigned)&OS.vdslst);
  asm("lda # >%v", dli1);
  asm("sta %w", ((unsigned)&OS.vdslst) + 1);

  asm("pla");
  asm("rti");
}

void dli1(void) {
  /* Push registers */
  asm("pha");
  asm("txa");
  asm("pha");

  asm("sta %w", (unsigned)&ANTIC.wsync);

  /* Get hue for fire */
  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("and $605"); // Truncate high bits (0x0F in normal situations)
  asm("adc $602"); // Add yellow
  asm("tax");

  /* 1st Row */

  /* - set font (1st row) */
  asm("lda $607");
  asm("sta %w", (unsigned)&ANTIC.chbase);

  /* - color gradient (1st row) */
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

  /* Chain to the next "2nd row" DLI */
  asm("lda # <%v", dli2);
  asm("sta %w", (unsigned)&OS.vdslst);
  asm("lda # >%v", dli2);
  asm("sta %w", ((unsigned)&OS.vdslst) + 1);

  /* Pop registers & return */

  asm("pla");
  asm("tax");
  asm("pla");

  asm("rti");
}

void dli2(void) {
  /* Push registers */
  asm("pha");
  asm("txa");
  asm("pha");

  asm("sta %w", (unsigned)&ANTIC.wsync);

  /* Get hue for fire */
  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("and $605"); // Truncate high bits (0x0F in normal situations)
  asm("adc $602"); // Add yellow
  asm("adc #8");
  asm("tax");

  /* 2nd Row */

  /* - set font (2nd row) */
  asm("lda $608");
  asm("sta %w", (unsigned)&ANTIC.chbase);

  /* - color gradient (1st row) */
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

  /* Chain to the next "1st row" DLI */
  asm("lda # <%v", dli1);
  asm("sta %w", (unsigned)&OS.vdslst);
  asm("lda # >%v", dli1);
  asm("sta %w", ((unsigned)&OS.vdslst) + 1);

  /* Pop registers & return */

  asm("pla");
  asm("tax");
  asm("pla");

  asm("rti");
}

void dli_vbi(void) {
  /* Start display with the first DLI */
  asm("lda # <%v", dli);
  asm("sta %w", (unsigned)&OS.vdslst);
  asm("lda # >%v", dli);
  asm("sta %w", ((unsigned)&OS.vdslst) + 1);

  /* Flip between the game character sets */
  asm("lda %w", (unsigned)&OS.rtclok[2]);
  asm("lsr"); // Slow it down
  asm("lsr");
  asm("and $606"); // Truncate all but "8" bit (0x08 in normal situations)
  asm("adc $600"); // Add the character set base (is there a better way of doing this? -bjk 2023.08.13)
  asm("adc #2"); // Add offset into top-half font
  asm("sta $607"); // Store it, for the main DLI and DLI #1 to use
  asm("adc #4"); // Add more offset into bottom-half font
  asm("sta $608"); // Store it, for DLI #2 to use

  asm("jmp (%v)", OLDVEC);
}

#ifdef DISK

/* High score table DLI */
void hs_dli(void) {
  asm("pha");

  asm("lda #10");

  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);
  asm("adc #2");

  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);
  asm("adc #2");

  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);
  asm("sbc #2");

  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);
  asm("sbc #2");

  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&ANTIC.wsync);
  asm("sta %w", (unsigned)&GTIA_WRITE.colpf1);
  asm("sbc #2");

  asm("pla");
  asm("rti");
}

#endif

