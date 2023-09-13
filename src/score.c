/*
  Firefighter scoring & high score functions

  Firefighting game for the Atari 8-bit
  Bill Kendrick <bill@newbreedsoftware.com>
  http://www.newbreedsoftware.com/firefighter/

  If "DISK" is defined, the game will support 10 high scores
  and save the results to disk.  Otherwise, it will keep a
  single high score, which will be lost when the game quits.

  Note: This is defined via compile-time options by the disk-based
  (".atr") version of the game.

  Further, if "HIGHSCORE_FILE" is defined, the game will
  use standard C `fopen()`/`fread()`/`fwrite()`/etc. functions
  to create a file named "highscor.dat" on disk to store the
  high scores.  Otherwise (the default) it will do direct
  sector I/O operations to load/save data on sector 720,
  and assumes the disk is Single Density (128 bytes per sector).
  (This allows it to remain in the same place on disk, regardless
  of other changes during each release, allowing for extraction
  of the high scores from the disk image, e.g. by a TNFS server
  to allow sharing a high score table by users playing over FujiNet.)

  h/t Thomas Cherryholmes for sharing example code.

  2023-08-27 - 2023-09-12
*/

#include <string.h>
#include <peekpoke.h>
#include <atari.h>

#ifdef DISK
#include <stdio.h>
#endif

#include "game.h"
#include "draw_text.h"
#include "score.h"

unsigned long int score, high_score;
char high_score_name[4];
char initials[4];

extern unsigned char scr_mem[];

#ifdef DISK
unsigned long int high_score_table[10];
char high_score_name_table[10][4];

char * default_high_initials = "BJK" "KAY" "BOB" "HVN" "HRD" "WIL" "EDW" "MEL" "M.G" "DAV";
unsigned char fade[16] = {
  0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E,
  0x4E, 0x4C, 0x4A, 0x48, 0x46, 0x44, 0x42, 0x40,
};

extern unsigned char * dlist;
extern unsigned char font1_data[];
#endif


/* Index into this array with an `OS.ch` keycode to get the
 * ATASCII equivalent (it is padded with "-" for unsupported codes) */
char key_to_ch[] = {
  'L', /* KEY_L           (0x00) */
  'J', /* KEY_J           (0x01) */
  ';', /* KEY_SEMICOLON   (0x02) */
  '-', /*                 (0x03) */
  '-', /*                 (0x04) */
  'K', /* KEY_K           (0x05) */
  '+', /* KEY_PLUS        (0x06) */
  '*', /* KEY_ASTERISK    (0x07) */
  'O', /* KEY_O           (0x08) */
  '-', /*                 (0x09) */
  'P', /* KEY_P           (0x0A) */
  'U', /* KEY_U           (0x0B) */
  '-', /*                 (0x0C) */
  'I', /* KEY_I           (0x0D) */
  '-', /* KEY_DASH        (0x0E) */
  '=', /* KEY_EQUALS      (0x0F) */

  'V', /* KEY_V           (0x10) */
  '-', /*                 (0x11) */
  'C', /* KEY_C           (0x12) */
  '-', /*                 (0x13) */
  '-', /*                 (0x14) */
  'B', /* KEY_B           (0x15) */
  'X', /* KEY_X           (0x16) */
  'Z', /* KEY_Z           (0x17) */
  '4', /* KEY_4           (0x18) */
  '-', /*                 (0x19) */
  '3', /* KEY_3           (0x1A) */
  '6', /* KEY_6           (0x1B) */
  '-', /*                 (0x1C) */
  '5', /* KEY_5           (0x1D) */
  '2', /* KEY_2           (0x1E) */
  '1', /* KEY_1           (0x1F) */

  ',', /* KEY_COMMA       (0x20) */
  ' ', /* KEY_SPACE       (0x21) */
  '.', /* KEY_PERIOD      (0x22) */
  'N', /* KEY_N           (0x23) */
  '-', /*                 (0x24) */
  'M', /* KEY_M           (0x25) */
  '/', /* KEY_SLASH       (0x26) */
  '-', /*                 (0x27) */
  'R', /* KEY_R           (0x28) */
  '-', /*                 (0x29) */
  'E', /* KEY_E           (0x2A) */
  'Y', /* KEY_Y           (0x2B) */
  '-', /*                 (0x2C) */
  'T', /* KEY_T           (0x2D) */
  'W', /* KEY_W           (0x2E) */
  'Q', /* KEY_Q           (0x2F) */

  '9', /* KEY_9           (0x30) */
  '-', /*                 (0x31) */
  '0', /* KEY_0           (0x32) */
  '7', /* KEY_7           (0x33) */
  '-', /*                 (0x34) */
  '8', /* KEY_8           (0x35) */
  '<', /* KEY_LESSTHAN    (0x36) */
  '>', /* KEY_GREATERTHAN (0x37) */
  'F', /* KEY_F           (0x38) */
  'H', /* KEY_H           (0x39) */
  'D', /* KEY_D           (0x3A) */
  '-', /*                 (0x3B) */
  '-', /*                 (0x3C) */
  'G', /* KEY_G           (0x3D) */
  'S', /* KEY_S           (0x3E) */
  'A', /* KEY_A           (0x3F) */
};

/* Test first sub-item against `OS.ch` keycode; if you find one,
 * the second sub-item is the ATASCII equivalent.
 * (Doing it this way since there are so few Shift+Key codes
 * that we need to support; it keeps the array size down,
 * in exchange for a small for-loop to do the look up.)
 */
#define NUM_SHIFTED_KEYS 17
char key_shift_to_ch_map[NUM_SHIFTED_KEYS][2] = {
  { KEY_EXCLAMATIONMARK, '!' },
  { KEY_QUOTE,           '"' },
  { KEY_HASH,            '#' },
  { KEY_DOLLAR,          '$' },
  { KEY_PERCENT,         '%' },
  { KEY_AMPERSAND,       '&' },
  { KEY_APOSTROPHE,      '\'' },
  { KEY_AT,              '@' },
  { KEY_OPENINGPARAN,    '(' },
  { KEY_CLOSINGPARAN,    ')' },
  { KEY_COLON,           ':' },
  { KEY_BACKSLASH,       '\\' },
  { KEY_OPENINGBRACKET,  '[' },
  { KEY_CLOSINGBRACKET,  ']' },
  { KEY_QUESTIONMARK,    '?' },
};


/* Local function prototype: */
void get_initials(void);

#ifdef DISK
#ifndef HIGHSCORE_FILE
void write_sector(unsigned short s, unsigned char *buf, unsigned short len);
void read_sector(unsigned short s, unsigned char *buf, unsigned short len);
void siov(void);
#endif
#endif

/* Sets default high score and (on disk version)
   top 10 high score table entries */
void set_default_high_score(void) {
#ifdef DISK
  int i;
#endif

  high_score = 1030;
  strcpy(high_score_name, "BJK");

#ifdef DISK
  high_score_table[0] = high_score;
  strcpy(high_score_name_table[0], high_score_name);

  for (i = 1; i < 10; i++) {
    high_score_table[i] = 1030 - (i * 100);
    high_score_name_table[i][0] = default_high_initials[i * 3 + 0];
    high_score_name_table[i][1] = default_high_initials[i * 3 + 1];
    high_score_name_table[i][2] = default_high_initials[i * 3 + 2];
    high_score_name_table[i][3] = '\0';
  }
#endif
}


/* Determine if they got a/the high score, get their initials, and
   update the high score (table).  (See `get_initials()`.)

   @return char position in high score table (or 0 if there is none)
     if they got a (the) high score, -1 otherwise
*/
char register_high_score(void) {
#ifdef DISK
  int i, better_than;

  /* Disk: Matched or beat one of the top 10 high scores? */
  better_than = -1;
  for (i = 0; i < 10 && better_than == -1; i++) {
    if (score >= high_score_table[i]) {
      better_than = i;
    }
  }

  if (better_than != -1) {
    get_initials();

    /* Move lower scores down a notch */
    for (i = 9; i > better_than; i--) {
      high_score_table[i] = high_score_table[i - 1];
      strcpy(high_score_name_table[i], high_score_name_table[i - 1]);
    }

    /* Insert our score */
    strcpy(high_score_name_table[better_than], initials);
    high_score_table[better_than] = score;

    if (better_than == 0) {
      /* Highest score */
      strcpy(high_score_name, initials);
      high_score = score;
    }

    return better_than;
  }
#else
  /* Non-disk: Matched or beat high score? */
  if (score >= high_score) {
    get_initials();
    strcpy(high_score_name, initials);
    high_score = score;

    return 0;
  }
#endif

  return -1;
}


/* Ask user for their initials, store in `initials` variable. */
void get_initials(void) {
  char x, done, s, i, ch;

  bzero(scr_mem + 60, LEVEL_SPAN);

  draw_text("HIGH! INITIALS: --- ", scr_mem);

  x = 0;
  strcpy(initials, "AAA");

  done = 0;
  do {
    /* Draw initials (highlighting the chosen) */
    for (i = 0; i < 3; i++) {
      POKE(scr_mem + 16 + i, initials[i] - 32 + (x == i ? 128 : 0));
    }

    /* Draw (or not) "EOL" at the end */
    if (x < 3) {
      POKE(scr_mem + 19, 0);
    } else {
      POKE(scr_mem + 19, 63 + 128);
    }

    /* Wait for input */
    do {
      s = (OS.stick0 != 15 ? OS.stick0 : OS.stick1);
      ch = OS.ch;
    } while (s == 15 && ch == KEY_NONE);

    if (x < 3) {
      if (s == 14) {
        /* [JS-Up]: Cycle letter backward */
        if (initials[x] > 32)
          initials[x]--;
        else
          initials[x] = 93;
      } else if (s == 13) {
        /* [JS-Down]: Cycle letter forward */
        if (initials[x] < 93)
          initials[x]++;
        else
          initials[x] = 32;
      } else if (s == 7 || OS.strig0 == 0 || OS.strig1 == 0) {
        /* [JS-Right] or [Fire]: Next initial */
        x++;
      } else {
        if (ch == KEY_RETURN) {
          /* [Return] (in initials): Jump to "EOL" */
          x = 3;
        } else if (ch <= 0x3F && ch != KEY_DELETE) {
          /* Unshifted Character input */
          initials[x++] = key_to_ch[ch];
        } else if (ch >= 0x40 && ch <= 0x7F) {
          /* Shifted Character input: */
          char found = 0;

          for (i = 0; i < NUM_SHIFTED_KEYS && !found; i++) {
            if (key_shift_to_ch_map[i][0] == ch) {
              initials[x++] = key_shift_to_ch_map[i][1];
              found = 1;
            }
          }
        }
      }
    } else {
      /* [JS-Fire] or [Return] on "EOL" symbol: All done! */
      if (OS.strig0 == 0 || OS.strig1 == 0 || ch == KEY_RETURN) {
        done = 1;
      }
    }

    if (x > 0 && (s == 11 || ch == KEY_DELETE)) {
      /* [JS-Left] or [Backspace]: Prev. initial */
      x--;
    }

    /* (Eat input) */
    do {
    } while (OS.stick0 != 15 || OS.stick1 != 15 || OS.strig0 == 0 || OS.strig1 == 0);
    OS.ch = KEY_NONE;
  } while (!done);
}


#ifdef DISK
/* Display the high score table
   @param highlight which entry to highlight (-1 for none) */
void show_high_score_table(char highlight) {
  int i;

  OS.sdmctl = 0;

  bzero(scr_mem, 1024);

  /* Generate a display list... */
  POKE(dlist, DL_BLK8);
  POKE(dlist + 1, DL_BLK8);
  POKE(dlist + 2, DL_BLK8);

  POKE(dlist + 3, DL_LMS(DL_GRAPHICS2));
  POKEW(dlist + 4, (unsigned int) scr_mem);

  POKE(dlist + 6, DL_BLK2);

  for (i = 7; i < 29; i++) {
    POKE(dlist + i, DL_GRAPHICS0);
  }

  POKE(dlist + 29, DL_BLK1);
  POKE(dlist + 30, DL_GRAPHICS0);

  POKE(dlist + 31, DL_JVB);
  POKEW(dlist + 32, (unsigned int) dlist);

  OS.sdlst = dlist;

  OS.color1 = 0x0F;
  OS.color2 = 0x44;
  OS.color4 = 0x42;

  OS.chbas = 0xE0; // OS ROM default character set
  OS.chbas = (unsigned char) ((unsigned int) font1_data / 256);

  OS.sdmctl = (DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_FETCH);

  /* Title */
  draw_text("TOP TEN FIREFIGHTERS", scr_mem);

  /* Draw high scores */
  for (i = 0; i < 10; i++) {
    draw_number(i + 1, 2, scr_mem + (i * 80) + 60 + 12);

    draw_text(high_score_name_table[i], scr_mem + (i * 80) + 60 + 16);
    draw_number(high_score_table[i], 6, scr_mem + (i * 80) + 60 + 21);

    if (i == highlight) {
      int j;
      for (j = 0; j < 40; j++) {
        POKE(scr_mem + (i * 80) + 60 + j, PEEK(scr_mem + (i * 80) + 60 + j) + 128);
      }
    }
  }

  /* Instructions */
  draw_text("SPACE/FIRE/ESCAPE: EXIT", scr_mem + (22 * 40) + 20 + 8);

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;

  /* Wait for input */
  do {
    OS.color0 = fade[(OS.rtclok[2] >> 2) & 0x0F];
  } while (OS.strig0 == 1 && OS.strig1 == 1 && CONSOL_START(GTIA_READ.consol) == 0 && OS.ch != KEY_SPACE && OS.ch != KEY_ESC);

  /* (Eat any input) */
  do {
  } while (OS.strig0 == 0 || OS.strig1 == 0 || CONSOL_START(GTIA_READ.consol) == 1);
  OS.ch = KEY_NONE;
}

/* Load high scores from disk
   (into high_score_table[] and high_score_name_table[]) */

#ifdef HIGHSCORE_FILE
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
#else
void load_high_scores(void) {
  unsigned char buf[128];

  read_sector(720, buf, 128);

  if (buf[0] == 0) {
    /* Seems to be nothing in this sector yet; ignore it! */
    return;
  }

  memcpy(high_score_table, buf, sizeof(high_score_table));
  memcpy(high_score_name_table, buf + sizeof(high_score_table), sizeof(high_score_name_table));
}
#endif

/* Save high scores to disk
   (from high_score_table[] and high_score_name_table[]) */
#ifdef HIGHSCORE_FILE
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
#else
void save_high_scores(void) {
  unsigned char buf[128];

  memcpy(buf, high_score_table, sizeof(high_score_table));
  memcpy(buf + sizeof(high_score_table), high_score_name_table, sizeof(high_score_name_table));

  write_sector(720, buf, 128);
}
#endif

#ifndef HIGHSCORE_FILE
void write_sector(unsigned short s, unsigned char *buf, unsigned short len)
{
    OS.dcb.ddevic = 0x31;
    OS.dcb.dunit = 0x01;
    OS.dcb.dcomnd = 'W';
    OS.dcb.dstats = 0x80;
    OS.dcb.dbuf = buf;
    OS.dcb.dtimlo = 0x0f;
    OS.dcb.dbyt = len;
    OS.dcb.daux = s;
    siov();
}

void read_sector(unsigned short s, unsigned char *buf, unsigned short len)
{
    OS.dcb.ddevic = 0x31;
    OS.dcb.dunit = 0x01;
    OS.dcb.dcomnd = 'R';
    OS.dcb.dstats = 0x40;
    OS.dcb.dbuf = buf;
    OS.dcb.dtimlo = 0x0f;
    OS.dcb.dbyt = len;
    OS.dcb.daux = s;
    siov();
}

/*
siov.s
        ;; Call SIO

        .export _siov

_siov:  JSR $E459               ; Call SIOV
        LDA $0303               ; get/return DVSTAT
        RTS                     ; Bye
*/

void siov(void) {
  asm("JSR $E459");
  asm("LDA $0303");
}
#endif

#endif
