/*
  Firefighter scoring & high score functions

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  2023-08-27 - 2023-08-27
*/

#include <string.h>

#ifdef DISK
#include <stdio.h>
#include <atari.h>
#include <peekpoke.h>
#include "draw_text.h"
#endif

#include "score.h"

unsigned long int score, high_score;
char high_score_name[4];
char initials[4];

#ifdef DISK
unsigned long int high_score_table[10];
char high_score_name_table[10][4];

extern unsigned char scr_mem[];
extern unsigned char * dlist;
#endif

void get_initials(void);


/* Sets default high score and (on disk version)
   top 10 high score table entries */
void set_default_high_score(void) {
#ifdef DISK
  int i;
#endif

  high_score = 1031;
  strcpy(high_score_name, "BJK");

#ifdef DISK
  high_score_table[0] = high_score;
  strcpy(high_score_name_table[0], high_score_name);

  for (i = 1; i < 10; i++) {
    high_score_table[i] = 1031 - (i * 100);
    strcpy(high_score_name_table[i], "BJK");
  }
#endif
}


/* Determine if they got a/the high score, get their initials, and
   update the high score (table).

   @return char 1 if they got a high score, 0 otherwise
*/
char register_high_score(void) {
#ifdef DISK
  int i;

  /* Disk: Matched or beat one of the top 10 high scores? */

  /* FIXME: Insert name in high score table */
#else
  /* Non-disk: Matched or beat high score? */
  if (score >= high_score) {
    get_initials();
    strcpy(high_score_name, initials);
    high_score = score;

    return 1;
  }
#endif

  return 0;
}


/* Ask user for their initials, store in `initials` variable */
void get_initials(void) {
  /* FIXME: */

  strcpy(initials, "NEW");
}


#ifdef DISK
/* Display the high score table */
void show_high_score_table(void) {
  int i;

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS0));
  POKEW(dlist + 4, (unsigned int) scr_mem);

  for (i = 6; i < 28; i++) {
    POKE(dlist + i, DL_GRAPHICS0);
  }

  POKE(dlist + 28, DL_BLK1);
  POKE(dlist + 29, DL_GRAPHICS0);

  POKE(dlist + 30, DL_JVB);
  POKEW(dlist + 31, (unsigned int) dlist);

  OS.sdlst = dlist;

  OS.color1 = 0x0F;
  OS.color2 = 0x44;
  OS.color4 = 0x42;

  OS.chbas = 0xE0; // OS ROM default character set

  OS.sdmctl = (DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH);

  /* Draw high scores */
  for (i = 0; i < 10; i++) {
    draw_text(high_score_name_table[i], scr_mem + i * 80 + 40 + 10);
    draw_number(high_score_table[i], 6, scr_mem + i * 80 + 40 + 15);
  }

  /* Instructions */
  draw_text("SPACE/FIRE/ESCAPE: Exit", scr_mem + (23 * 40) + 8);

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;

  /* Wait for input */
  do {
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0 && OS.ch != KEY_SPACE && OS.ch != KEY_ESC);

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;
}

/* Load high scores from disk
   (into high_score_table[] and high_score_name_table[]) */
void load_high_scores(void) {
  int i;
  FILE * fi;

  fi = fopen("highscor.dat", "rb");
  if (fi != NULL) {
    bzero(high_score_name_table, sizeof(char) * 4 * 10);
    for (i = 0; i < 10; i++) {
      fread(&(high_score_name_table[i]), sizeof(char), 3, fi);
      fread(&(high_score_table[i]), sizeof(high_score), 1, fi);
    }

    fclose(fi);

    /* Copy the top score into the variables used to
       display the top score on the title screen
       (which, in non-disk version, is all there is) */
    high_score = high_score_table[0];
    strcpy(high_score_name, high_score_name_table[0]);
  }
}

/* Save high scores to disk
   (from high_score_table[] and high_score_name_table[]) */
void save_high_scores(void) {
  int i;
  FILE * fi;

  fi = fopen("highscor.dat", "wb");
  if (fi != NULL) {
    for (i = 0; i < 10; i++) {
      fwrite(&(high_score_name_table[i]), sizeof(char), 3, fi);
      fwrite(&(high_score_table[i]), sizeof(high_score), 1, fi);
    }

    fclose(fi);
  }
}

#endif

